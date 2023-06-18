//Torlus - added DISABLE_VERIFIER option

/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Internal runtime structures
 * FILE:      class.c
 * OVERVIEW:  Internal runtime class structures (see Class.h).
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Edited by Doug Simon 11/1998 (added the string pool)
 *            Added access checks by Sheng Liang for VM-spec compliance
 *            Frank Yellin (more checks for JLS compliance)
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>
#include <stddef.h>

/*=========================================================================
 * Global variables and definitions
 *=======================================================================*/

/* Pointers to the most important Java classes needed by the VM */
/* If Romizing, these are defined and initialized in ROMjava.c */

#if ROMIZING
#define EXTERN_IF_ROMIZING extern
#else
#define EXTERN_IF_ROMIZING
#endif

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangObject;    /* Pointer to class 'java.lang.Object' */

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangClass;     /* Pointer to class 'java.lang.Class' */

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangSystem;    /* Pointer to class 'java.lang.System' */

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangString;    /* Pointer to class 'java.lang.String' */

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangThread;    /* Pointer to class 'java.lang.Thread' */

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangThrowable; /* Pointer to class 'java.lang.Throwable' */

EXTERN_IF_ROMIZING
INSTANCE_CLASS JavaLangError;     /* Pointer to class 'java.lang.Error' */

EXTERN_IF_ROMIZING METHOD RunCustomCodeMethod;

EXTERN_IF_ROMIZING NameTypeKey initNameAndType;   /* void <init>() */
EXTERN_IF_ROMIZING NameTypeKey clinitNameAndType; /* void <clinit>() */
EXTERN_IF_ROMIZING NameTypeKey runNameAndType;    /* void run() */
EXTERN_IF_ROMIZING NameTypeKey mainNameAndType;   /* void main(String[]) */

EXTERN_IF_ROMIZING ARRAY_CLASS PrimitiveArrayClasses[T_LASTPRIMITIVETYPE + 1];

INSTANCE_CLASS JavaLangOutOfMemoryError;
THROWABLE_INSTANCE OutOfMemoryObject;
THROWABLE_INSTANCE StackOverflowObject;

/*=========================================================================
 * Static methods (only used in this file)
 *=======================================================================*/

static void runClinit(FRAME_HANDLE);
static void runClinitException(FRAME_HANDLE);