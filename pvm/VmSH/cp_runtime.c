// PC

#include <global.h>
#include <stdlib.h>

void AlertUser(const char *message) {
  Debug_Printf(0, 10, 0, 0, "%s", message);
  LCD_Refresh();
}

jcell *allocateHeap(long *sizeptr, void **realresultptr) { 
    void *space = malloc(*sizeptr + sizeof(jcell) - 1);
    *realresultptr = space;
    return (void *) ((((long)space) + (sizeof(jcell) - 1)) & ~(sizeof(jcell) - 1));
}


void *
allocateVirtualMemory_md(long size) {
}

void 
freeVirtualMemory_md(void *address, long size) { 
}

void  
protectVirtualMemory_md(void *address, long size, int protection) {
}

static void signal_handler(int sig) {
}

void InitializeFloatingPoint() {
}

void InitializeNativeCode() {
}

void FinalizeNativeCode() {
}