
/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Memory management
 * FILE:      collector.c
 * OVERVIEW:  Exact, compacting garbage collector.
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Edited by Doug Simon 11/1998 (made collection more precise)
 *            Frank Yellin (removed excessive recursion)
 *            Frank Yellin, Matt Seidl (exact, compacting GC)
 *            Weak reference support for CLDC 1.1, A. Taivalsaari 3/2002
 *=======================================================================*/

/*=========================================================================
 * For detailed explanation of the memory system, see Garbage.h
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>

/*=========================================================================
 * Definitions and declarations
 *=======================================================================*/

#define PTR_OFFSET(ptr, offset) ((void *)((char *)ptr + offset))
#define PTR_DELTA(ptr1, ptr2) ((char *)(ptr1) - (char *)ptr2)

#define CELL_AT_OFFSET(ptr, offset) (*(jcell *)PTR_OFFSET(ptr, offset))

#if ENABLE_HEAP_COMPACTION
# define ISKEPT(n) ((n) & MARKBIT)
#else
# define ISKEPT(n) ((n) & (MARKBIT|STATICBIT))
#endif

typedef struct breakTableStruct {
    int length;                 /* in entries */
    struct breakTableEntryStruct *table;
} breakTableStruct;

typedef struct breakTableEntryStruct {
    jcell *address;
    int offset;
} breakTableEntryStruct;

/*=========================================================================
 * Variables
 *=======================================================================*/

void* TheHeap;
long  VMHeapSize;               /* Heap size */

jcell* AllHeapStart;             /* Heap bottom */
jcell* CurrentHeap;              /* Same as AllHeapStart*/
jcell* CurrentHeapEnd;           /* End of heap */
jcell* AllHeapEnd;

WEAKPOINTERLIST WeakPointers;   /* List of weak pointers during GC */
WEAKREFERENCE   WeakReferences; /* List of weak refs during GC (CLDC 1.1) */

CHUNK FirstFreeChunk;

#if ENABLE_HEAP_COMPACTION
jcell* PermanentSpaceFreePtr;
#endif

#define DEFERRED_OBJECT_TABLE_SIZE 40
static jcell *deferredObjectTable[DEFERRED_OBJECT_TABLE_SIZE];
#define endDeferredObjectTable (deferredObjectTable + DEFERRED_OBJECT_TABLE_SIZE)
static jcell **startDeferredObjects,  **endDeferredObjects;
static int deferredObjectCount;
static int deferredObjectTableOverflow;

/*=========================================================================
 * Static functions (private to this file)
 *=======================================================================*/

static void putDeferredObject(jcell *c);
static jcell* getDeferredObject(void);
static void initializeDeferredObjectTable(void);
static void markChildren(jcell* object, jcell* limit, int remainingDepth);
static void checkMonitorAndMark(OBJECT object);

static jcell* allocateFreeChunk(long size);

static CHUNK sweepTheHeap(long *maximumFreeSizeP);

/*=========================================================================
 * Heap initialization operations
 *=======================================================================*/

void InitializeHeap(void)
{
    VMHeapSize = RequestedHeapSize;
    AllHeapStart = allocateHeap(&VMHeapSize, &TheHeap);

    if (TheHeap == NIL) {
        fatalVMError(KVM_MSG_NOT_ENOUGH_MEMORY);
    }

    /* Initially, don't create any permanent space.  It'll grow as needed */
    CurrentHeap    = AllHeapStart;
    CurrentHeapEnd = PTR_OFFSET(AllHeapStart, VMHeapSize);

#if !CHUNKY_HEAP
    FirstFreeChunk = (CHUNK)CurrentHeap;
    FirstFreeChunk->size =
           (CurrentHeapEnd -CurrentHeap - HEADERSIZE) << TYPEBITS;
    FirstFreeChunk->next = NULL;
#endif

    /* Permanent space goes from CurrentHeapEnd to AllHeapEnd.  It currently
     * has zero size.
     */
#if ENABLE_HEAP_COMPACTION
    PermanentSpaceFreePtr = CurrentHeapEnd;
#endif
    AllHeapEnd            = CurrentHeapEnd;

#if INCLUDEDEBUGCODE
    if (tracememoryallocation) {
        Log->allocateHeap(VMHeapSize, (long)AllHeapStart, (long)AllHeapEnd);
    }
    NoAllocation = 0;
#endif
}


/*=========================================================================
 * Memory allocation operations
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      mallocHeapObject()
 * TYPE:          public memory allocation operation
 * OVERVIEW:      Allocate a contiguous area of n cells in the dynamic heap.
 * INTERFACE:
 *   parameters:  size: the requested object size in cells,
 *                type: garbage collection type of the object
 *   returns:     pointer to newly allocated area, or
 *                NIL is allocation fails.
 * COMMENTS:      Allocated data area is not initialized, so it
 *                may contain invalid heap references. If you do
 *                not intend to initialize data area right away,
 *                you must use 'callocObject' instead (or otherwise
 *                the garbage collector will get confused next time)!
 *=======================================================================*/

jcell* mallocHeapObject(long size, GCT_ObjectType type)
/* Remember: size is given in CELLs rather than bytes */
{
    jcell* thisChunk;
    long realSize;

    if (size == 0) size = 1;
    realSize = size + HEADERSIZE;

#if INCLUDEDEBUGCODE
    if (NoAllocation > 0) {
        fatalError(KVM_MSG_CALLED_ALLOCATOR_WHEN_FORBIDDEN);
    }
#endif /* INCLUDEDEBUGCODE */

    if (EXCESSIVE_GARBAGE_COLLECTION) {
        garbageCollect(0);
    }

    thisChunk = allocateFreeChunk(realSize);
    if (thisChunk == NULL) {
        garbageCollect(realSize); /* So it knows what we need */
        thisChunk = allocateFreeChunk(realSize);
        if (thisChunk == NULL) {
            return NULL;
        }
    }

    /* Augment the object header with gc type information */
    /* NOTE: This operation does not set the size of the object!! */
    /* You must initialize the size elsewhere, or otherwise the */
    /* memory system will be corrupted! */
    *thisChunk |= (type << TYPE_SHIFT);

#if INCLUDEDEBUGCODE
    if (tracememoryallocation) {
        Log->allocateObject((long)thisChunk,
                            (long)size+HEADERSIZE,
                            (int)type,
                            (long)thisChunk, memoryFree());
    }
#endif /* INCLUDEDEBUGCODE */

#if ENABLEPROFILING
    DynamicObjectCounter     += 1;
    DynamicAllocationCounter += (size+HEADERSIZE)*CELL;
#endif /* ENABLEPROFILING */

    return thisChunk + HEADERSIZE;
}


static jcell* allocateFreeChunk(long size)
{
    CHUNK thisChunk = FirstFreeChunk;
    CHUNK* nextChunkPtr = &FirstFreeChunk;
    jcell* dataArea  = NIL;

    for (thisChunk = FirstFreeChunk, nextChunkPtr = &FirstFreeChunk;
         thisChunk != NULL;
         nextChunkPtr = &thisChunk->next, thisChunk = thisChunk->next) {

        /* Calculate how much bigger or smaller the */
        /* chunk is than the requested size */
        long overhead = SIZE(thisChunk->size) + HEADERSIZE - size;

        if (overhead > HEADERSIZE) {
            thisChunk->size = (overhead - HEADERSIZE) << TYPEBITS;
            dataArea = (jcell *)thisChunk + overhead;
            *dataArea = (size - HEADERSIZE) << TYPEBITS;
            return dataArea;
        } else if (overhead >= 0) {
            /* There was an exact match or overhead is too small to be useful.
             * Remove this chunk from the free list, and if there is extra
             * space at the end of the chunk, it becomes wasted space for the
             * lifetime of the allocated object
             */
            *nextChunkPtr = thisChunk->next;
            dataArea = (jcell *)thisChunk;
            /* Store the size of the object in the object header */
            *dataArea = (size + overhead - HEADERSIZE) << TYPEBITS;
            return dataArea;
        }
    }
    /* If we got here, there was no chunk with enough memory available */
    return NULL;
}
