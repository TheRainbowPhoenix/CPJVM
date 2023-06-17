/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Global definitions
 * FILE:      global.h
 * OVERVIEW:  Global system-wide definitions.
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Many others since then...
 *=======================================================================*/

/*=========================================================================
 * COMMENT:
 * This file contains declarations that do not belong to any
 * particular structure or subsystem of the VM. There are many
 * other additional global variable declarations in other files.
 *=======================================================================*/

/*=========================================================================
 * System include files
 *=======================================================================*/

/* The performs per-machine initialization */
#include <machine_md.h>

/*=========================================================================
 * Global compile-time constants and typedefs
 *=======================================================================*/

#undef  TRUE
#undef  FALSE

#define NIL   0

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

/*=========================================================================
 * Global data type declarations
 *=======================================================================*/

/*=========================================================================
 * NOTE: These type declarations are not quite as portable as they
 * could be.  It might be useful to declare specific type names for
 * all Java-specific types (e.g., jint instead of normal int, jlong
 * instead of long64, and so forth).
 *=======================================================================*/

#define CELL  4        /* Size of a java word (= 4 bytes) */
#define log2CELL 2     /* Shift amount equivalent to dividing by CELL */
#define SHORTSIZE 2

typedef unsigned char     BYTE;
typedef unsigned long     cell;    /* Must be 32 bits long! */

/*=========================================================================
 * System-wide structure declarations
 *=======================================================================*/

typedef struct classStruct*         CLASS;
typedef struct instanceClassStruct* INSTANCE_CLASS;
typedef struct arrayClassStruct*    ARRAY_CLASS;

typedef struct objectStruct*        OBJECT;
typedef struct instanceStruct*      INSTANCE;
typedef struct arrayStruct*         ARRAY;