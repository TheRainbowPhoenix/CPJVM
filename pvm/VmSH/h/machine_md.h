//PC - based on VmUnix machine_md.h file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

#define DEFAULTHEAPSIZE 64*1024
#define JAR_FILES_USE_STDIO 0

#define COMPILER_SUPPORTS_LONG 0
// Turn this flag on if your compiler has support for long (64 bit) integers.

#define NEED_LONG_ALIGNMENT 0 
// Instructs the KVM to know that your host operating system and compiler generally assume all 64-bit integers to be aligned on eight-byte boundaries.

#define NEED_DOUBLE_ALIGNMENT 0 
// Instructs the KVM to know that your host operating system and compiler generally assume all double floating point numbers to be aligned on eight-byte boundaries (this flag is meaningful only if floating point support is turned on.)

#define BIG_ENDIAN 1
#define LITTLE_ENDIAN 0 

#define IMPLEMENTS_FLOAT 0

#define USE_JAM 0 

#define ASYNCHRONOUS_NATIVE_FUNCTIONS 0 

#define USE_KNI 0 // should be 1 later

#define InitializeVM()
#define FinalizeVM()

typedef long long long64;   
typedef unsigned long long ulong64;
typedef long long jlong;