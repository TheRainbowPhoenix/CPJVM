/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * KVM
 *=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Main Java program
 * FILE:      StartJVM.c
 * OVERVIEW:  System initialization and start of JVM
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Edited by Doug Simon 11/1998
 *            Frank Yellin
 *            Bill Pittore (Java-level debugging)
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>

/*=========================================================================
 * Functions
 *=======================================================================*/



/*=========================================================================
 * FUNCTION:      KVM_Cleanup
 * TYPE:          private operation
 * OVERVIEW:      Clean up everything.  This operation is called
 *                when the VM is shut down.
 * INTERFACE:
 *   parameters:  <none>
 *   returns:     <nothing>
 *=======================================================================*/

void KVM_Cleanup() {
  FinalizeVM();
//   FinalizeInlineCaching();
//   FinalizeNativeCode();
//   FinalizeJavaSystemClasses();
//   FinalizeClassLoading();
//   FinalizeMemoryManagement();
//   DestroyROMImage();
//   FinalizeHashtables();
}