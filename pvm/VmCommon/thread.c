/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Thread (concurrency) management
 * FILE:      thread.c
 * OVERVIEW:  This file defines the structures and operations
 *            that are needed for Java-style multithreading.
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Edited by Doug Simon 11/1998
 *            Edited by Nik Shaylor 09/1999 to allow asynchronous I/O
 *            Frank Yellin (new monitors), Bill Pittore (debugging)
 *=======================================================================*/

/*=========================================================================
 * COMMENTS:
 * Read the description of thread and monitor structures in thread.h
 *=======================================================================*/

/*=========================================================================
 * Local include files
 *=======================================================================*/

#include <global.h>
#include <execute.h>

/*=========================================================================
 * Global variables needed for multitasking
 *=======================================================================*/

THREAD CurrentThread;   /* Current thread pointer */
THREAD MainThread;      /* Global so debugger code can create a name */

THREAD AllThreads;      /* List of all threads */
THREAD RunnableThreads;  /* Runnable thread list */


/* NOTE:
 * RunnableThreads is a circular queue of threads.  RunnableThreads
 * is either NULL or points to the >>last<< element of the list.
 * This makes it easier to add to either end of the list *
 */

int AliveThreadCount;   /* Number of alive threads in AllThreads.  This count
                         * does >>not<< include threads that haven't yet been
                         * started */

int Timeslice;          /* Time slice counter for multitasking */

/*=========================================================================
 * Static declarations needed for this file
 *=======================================================================*/

#define QUEUE_ADDED TRUE
#define QUEUE_REMOVED FALSE

#if INCLUDEDEBUGCODE
static void
TraceMonitor(THREAD thread, MONITOR monitor, THREAD *queue, bool_t added);
static void TraceThread(THREAD thread, char *what);
#endif