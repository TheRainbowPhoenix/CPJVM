/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Memory management
 * FILE:      garbage.c
 * OVERVIEW:  Memory manager/garbage collector for the KVM.
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Edited by Doug Simon 11/1998
 *            Frank Yellin (exact collection, compaction)
 *            Compaction based on code written by Matt Seidl
 *=======================================================================*/

/*=========================================================================
 * For detailed explanation of the memory system, see Garbage.h
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>

/*=========================================================================
 * Variables
 *=======================================================================*/

int TemporaryRootsLength;
int GlobalRootsLength;
int gcInProgress;

union cellOrPointer TemporaryRoots[MAXIMUM_TEMPORARY_ROOTS];
union cellOrPointer GlobalRoots[MAXIMUM_GLOBAL_ROOTS];
POINTERLIST         CleanupRoots;



/*=========================================================================
 * Memory allocation operations
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      mallocObject, callocObject
 * TYPE:          public memory allocation operation
 * OVERVIEW:      Allocate a contiguous area of n cells in the dynamic heap.
 * INTERFACE:
 *   parameters:  size: the requested object size in cells,
 *                type: garbage collection type of the object
 *   returns:     pointer to newly allocated area.
 *                Raises an exception if space cannot be allocated.
 *
 * COMMENTS:      callocObject zeros the space before returning it.
 *=======================================================================*/

jcell* mallocObject(long size, GCT_ObjectType type)
/*  Remember: size is given in CELLs rather than bytes */
{
    jcell* result = mallocHeapObject(size, type);
    if (result == NULL) {
        THROW(OutOfMemoryObject);
    }

    return result;
}

jcell* callocObject(long size, GCT_ObjectType type)
{
    jcell* result = mallocHeapObject(size, type);
    if (result == NULL) {
        THROW(OutOfMemoryObject);
    }

    /* Initialize the area to zero */
    memset(result, 0, size << log2CELL);

    return result;
}

/*=========================================================================
 * FUNCTION:      InitializeMemoryManagement()
 * TYPE:          public global operation
 * OVERVIEW:      Initialize the system memory heaps.
 * INTERFACE:
 *   parameters:  <none>
 *   returns:     <nothing>
 *=======================================================================*/

void InitializeMemoryManagement(void)
{
    int index;
    gcInProgress = 0;
    InitializeHeap();

    index = 0;
    GlobalRoots[index++].cellpp = (jcell **)&AllThreads;
    GlobalRoots[index++].cellpp = (jcell **)&CleanupRoots;
    GlobalRootsLength = index;
    TemporaryRootsLength = 0;

    CleanupRoots =
          (POINTERLIST)callocObject(SIZEOF_POINTERLIST(CLEANUP_ROOT_SIZE),
                                    GCT_POINTERLIST);
}


void
garbageCollect(int moreMemory)
{
#if INCLUDEDEBUGCODE
    int beforeCollection = 0;
    int afterCollection = 0;
#endif

//     if (gcInProgress != 0) {
//         /* 
//          * Circular invocation of GC should not be allowed
//          */
//         fatalVMError(KVM_MSG_CIRCULAR_GC_INVOCATION);
//     }

//     gcInProgress++;

//     RundownAsynchronousFunctions();

//     if (ENABLEPROFILING && INCLUDEDEBUGCODE) {
//         checkHeap();
//     }
// #if INCLUDEDEBUGCODE
//     if ((tracegarbagecollection || tracegarbagecollectionverbose)
//         && !TERSE_MESSAGES) {
//             Log->startGC();
//     }
// #endif

// #if INCLUDEDEBUGCODE
//     if (ENABLEPROFILING || tracegarbagecollection
//         || tracegarbagecollectionverbose) {

//         beforeCollection = memoryFree();
//     }
// #endif

//     MonitorCache = NULL;        /* Clear any temporary monitors */

//     /* Store virtual machine registers of the currently active thread before
//      * garbage collection (must be  done to enable execution stack scanning).
//     */
//     if (CurrentThread) {
//         storeExecutionEnvironment(CurrentThread);
//     }

//     garbageCollectForReal(moreMemory);

//     if (CurrentThread) {
//         loadExecutionEnvironment(CurrentThread);
//     }

// #if INCLUDEDEBUGCODE
//     if (ENABLEPROFILING || tracegarbagecollection
//         || tracegarbagecollectionverbose) {

//         afterCollection = memoryFree();

// #if ENABLEPROFILING
//         GarbageCollectionCounter += 1;
//         DynamicDeallocationCounter += (afterCollection - beforeCollection);
// #endif

//         if (tracegarbagecollection || tracegarbagecollectionverbose) {

//             Log->endGC(afterCollection - beforeCollection, afterCollection, getHeapSize());
//         }
//     }
// #endif /* INCLUDEDEBUGCODE */

//     RestartAsynchronousFunctions();
//     /*
//      * Reset to indicate end of garbage collection
//      */
//     gcInProgress = 0;
}