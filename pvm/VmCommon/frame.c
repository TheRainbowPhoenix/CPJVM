/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

/*=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Interpreter stack frames
 * FILE:      frame.c
 * OVERVIEW:  This file defines the internal operations for
 *            manipulating stack frames & performing exception
 *            handling.
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Edited by Doug Simon 11/1998 (simplified exception handling)
 *            Sheng Liang (chunky stacks), Frank Yellin
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>

/*=========================================================================
 * Global variables and definitions
 *=======================================================================*/

/* Shortcuts to the errors/exceptions that the VM may throw */
/* These come directly from the CLDC Specification */

const char ArithmeticException[]
           = "java/lang/ArithmeticException";
const char ArrayIndexOutOfBoundsException[]
           = "java/lang/ArrayIndexOutOfBoundsException";
const char ArrayStoreException[]
           = "java/lang/ArrayStoreException";
const char ClassCastException[]
           = "java/lang/ClassCastException";
const char ClassNotFoundException[]
            = "java/lang/ClassNotFoundException";
const char IllegalAccessException[]
           = "java/lang/IllegalAccessException";
const char IllegalArgumentException[]
           = "java/lang/IllegalArgumentException";
const char IllegalMonitorStateException[]
           = "java/lang/IllegalMonitorStateException";
const char IllegalThreadStateException[]
           = "java/lang/IllegalThreadStateException";
const char IndexOutOfBoundsException[]
           = "java/lang/IndexOutOfBoundsException";
const char InstantiationException[]
           = "java/lang/InstantiationException";
const char InterruptedException[]
           = "java/lang/InterruptedException";
const char NegativeArraySizeException[]
           = "java/lang/NegativeArraySizeException";
const char NullPointerException[]
           = "java/lang/NullPointerException";
const char NumberFormatException[]
           = "java/lang/NumberFormatException";
const char RuntimeException[]
           = "java/lang/RuntimeException";
const char SecurityException[]
           = "java/lang/SecurityException";
const char StringIndexOutOfBoundsException[]
           = "java/lang/StringIndexOutOfBoundsException";

const char IOException[]
           = "java/io/IOException";

const char NoClassDefFoundError[]
           = "java/lang/NoClassDefFoundError";
const char OutOfMemoryError[]
           = "java/lang/OutOfMemoryError";
const char VirtualMachineError[]
           = "java/lang/VirtualMachineError";

/* The following classes would be needed for full JLS/JVMS compliance */
/* However, they are not part of the CLDC Specification. */

const char AbstractMethodError[]
           = "java/lang/AbstractMethodError";
const char ClassCircularityError[]
           = "java/lang/ClassCircularityError";
const char ClassFormatError[]
           = "java/lang/ClassFormatError";
const char IllegalAccessError[]
           = "java/lang/IllegalAccessError";
const char IncompatibleClassChangeError[]
           = "java/lang/IncompatibleClassChangeError";
const char InstantiationError[]
           = "java/lang/InstantiationError";
const char NoSuchFieldError[]
           = "java/lang/NoSuchFieldError";
const char NoSuchMethodError[]
           = "java/lang/NoSuchMethodError";
const char VerifyError[]
           = "java/lang/VerifyError";

/*
const char InternalError[]
           = "java/lang/InternalError";
const char ExceptionInInitializerError[]
           = "java/lang/ExceptionInInitializerError";
const char LinkageError[]
           = "java/lang/LinkageError";
const char StackOverflowError[]
           = "java/lang/StackOverflowError";
const char UnknownError[]
           = "java/lang/UnknownError";
const char UnsatisfiedLinkError[]
           = "java/lang/UnsatisfiedLinkError";
const char UnsupportedClassVersionError[]
           = "java/lang/UnsupportedClassVersionError";
*/

/*=========================================================================
 * Static function prototypes (for functions used only in this file)
 *=======================================================================*/

#if INCLUDEDEBUGCODE

static void exceptionThrownTracing(INSTANCE_HANDLE exceptionH);
static void exceptionCaughtTracing(THROWABLE_INSTANCE_HANDLE exceptionH, HANDLER handler);

#else

#define exceptionThrownTracing(exception)
#define exceptionCaughtTracing(exception, handler)

#endif /* INCLUDEDEBUGCODE */


/*=========================================================================
 * FUNCTION:      fatalVMError()
 * TYPE:          internal error handling operation
 * OVERVIEW:      Report a fatal error indicating that some severe
 *                unexpected situation has been encountered by the VM.
 *                This may be due to a bug in the VM.  VM execution will
 *                be stopped. This operation should be called only from
 *                inside the VM.
 * INTERFACE:
 *   parameters:  error message string
 *   returns:     <nothing>
 *=======================================================================*/

void fatalVMError(const char* errorMessage)
{
    fatalError(errorMessage);
}

/*=========================================================================
 * FUNCTION:      fatalError()
 * TYPE:          internal error handling operation
 * OVERVIEW:      Report a fatal error indicating that the execution
 *                of erroneous Java code might have endangered the
 *                integrity of the VM. VM will be stopped. This
 *                operation should be called only the from inside the VM.
 * INTERFACE:
 *   parameters:  error message string.
 *   returns:     <nothing>
 *=======================================================================*/

void fatalError(const char* errorMessage)
{
    if (INCLUDEDEBUGCODE) {
        printVMstatus();
    }
    AlertUser(errorMessage);
    VM_EXIT(FATAL_ERROR_EXIT_CODE);
}