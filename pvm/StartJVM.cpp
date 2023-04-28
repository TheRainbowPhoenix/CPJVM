#include <sdk/os/lcd.hpp>
#include <sdk/os/debug.hpp>
#include <stddef.h>

#include "./h/messages.h"

void AlertUser(const char* message) {
    Debug_Printf(0,10,false,0,"%s",message);
    LCD_Refresh();
}

/*=========================================================================
 * FUNCTION:      KVM_Cleanup
 * TYPE:          private operation
 * OVERVIEW:      Clean up everything.  This operation is called
 *                when the VM is shut down.
 * INTERFACE:
 *   parameters:  <none>
 *   returns:     <nothing>
 *=======================================================================*/

void KVM_Cleanup()
{
    /*
    FinalizeVM();
    FinalizeInlineCaching();
    FinalizeNativeCode();
    FinalizeJavaSystemClasses();
    FinalizeClassLoading();
    FinalizeMemoryManagement();
    DestroyROMImage();
    FinalizeHashtables();
    */
}

int StartJVM(int argc, char* argv[])
{
    volatile int returnValue = 0;

    /* Ensure that we have a class to run */
    if (argc <= 0 || argv[0] == NULL) {
        AlertUser(KVM_MSG_MUST_PROVIDE_CLASS_NAME);
        return -1;
    }


    AlertUser("TODO: KVM_Start");
    // returnValue = KVM_Start(argc, argv);
    // KVM_Cleanup();
    return returnValue;
}
