#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/file.hpp>

#include <string.h>
#include <stdlib.h>

#include "pvm/h/main.h"

#include <global.h>

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("Java VM")
APP_DESCRIPTION("A small JVM")
APP_AUTHOR("PC")
APP_VERSION("1.0.0")

// long RequestedHeapSize;
char* UserClassPath;


extern "C" void main()
{
	calcInit(); // backup screen and init some variables

	// Put your app's code here!

	// TEST JVM Types
	char c = 127;
	signed char sc = -128;
	unsigned char uc = 255;
	short s = -32768;
	unsigned short us = 65535;
	int i = -2147483648;
	unsigned int ui = 4294967295;
	long l = -2147483648;
	unsigned long ul = 4294967295;
	void *ptr = NULL;

	// Example for fillScreen(color);
	fillScreen(color(0, 64, 0));
	fillScreen(color(0, 64, 0));

	// Example for Debug_Printf(x,y,invert_color,0,format_string) //(small text)
	Debug_Printf(0, 1, false, 0, "%d", c);
	Debug_Printf(0, 2, false, 0, "%d", sc);
	Debug_Printf(0, 3, false, 0, "%d", uc);
	Debug_Printf(0, 4, false, 0, "%d", s);
	Debug_Printf(0, 5, false, 0, "%u", us);
	Debug_Printf(0, 6, false, 0, "%d", i);
	Debug_Printf(0, 7, false, 0, "%u", ui);
	Debug_Printf(0, 8, false, 0, "%ld", l);
	Debug_Printf(0, 9, false, 0, "%lu", ul);
	Debug_Printf(0, 10, false, 0, "%p", ptr);

	char str1[20] = "Hello";
	char str2[20] = "JVM";
	char *ptr1;
	char *ptr2;
	int cmp_result;
	int len;

	// String manipulation
	ptr1 = strcat(str1, str2);
	Debug_Printf(0, 11, false, 0, "strcat: %s\n", ptr1);

	ptr2 = strchr(str1, 'o');
	Debug_Printf(0, 12, false, 0, "strchr: %s\n", ptr2);

	cmp_result = strcmp(str1, str2);
	Debug_Printf(0, 13, false, 0, "strcmp: %d\n", cmp_result);

	strcpy(str1, str2);
	Debug_Printf(0, 14, false, 0, "strcpy: %s\n", str1);

	strncpy(str1, str2, 3);
	Debug_Printf(0, 15, false, 0, "strncpy: %s\n", str1);

	len = strlen(str1);
	Debug_Printf(0, 16, false, 0, "strlen: %d\n", len);

	// Memory manipulation
	char src[10] = "Hello";
	char dest[10];
	memcpy(dest, src, sizeof(src));
	Debug_Printf(0, 17, false, 0, "memcpy: %s\n", dest);

	char src2[10] = "World";
	char dest2[10];
	memmove(dest2, src2, sizeof(src2));
	Debug_Printf(0, 18, false, 0, "memmove: %s\n", dest2);

	char arr[10];
	memset(arr, 0, sizeof(arr));
	Debug_Printf(0, 19, false, 0, "memset: %s\n", arr);

	char arr1[10] = {1, 2, 3, 4, 5};
	char arr2[10] = {1, 2, 3, 4, 6};
	int cmp = memcmp(arr1, arr2, sizeof(arr1));
	Debug_Printf(0, 20, false, 0, "memcmp: %d\n", cmp);

	char a_str1[] = "1234";
	char a_str2[] = "5678";
	int num1, num2, sum;

	num1 = atoi(a_str1);
	num2 = atoi(a_str2);
	sum = num1 + num2;
	Debug_Printf(0, 21, false, 0, "sum = %d\n", sum);

	// use this command to actually update the screen
	LCD_Refresh();

	fillScreen(color(0, 0, 0));
	Debug_Printf(0, 0, false, 0, "Loading JVM Class");
	LCD_Refresh();

	// Load class

	char filename[] = "\\fls0\\jvm\\HelloWorld.class";

	int32_t class_file = open(filename, OPEN_READ);

	if (class_file < 0)
	{
		Debug_Printf(0, 1, true, 0, "Can't load %s", filename);
	}
	else
	{

		struct stat class_file_stat;

		fstat(class_file, &class_file_stat);

		// dynamically allocate space for bc (bytecode) class in heap
		uint8_t *bc = (uint8_t *)malloc(class_file_stat.fileSize);

		if (!bc)
		{
			Debug_Printf(0, 1, true, 0, "Class stat is null ?");
		}
		else
		{
			int32_t bytes_read = read(class_file, bc, class_file_stat.fileSize);

			if (bytes_read < 0)
			{
				Debug_Printf(0, 1, true, 0, "Empty file : %d", bytes_read);
			}
			else
			{
				Debug_Printf(0, 1, false, 0, "Read %d bytes of class - OK", bytes_read);

				RequestedHeapSize = DEFAULTHEAPSIZE;
    			UserClassPath = ".";

				char class_name[] = "HelloWorld";
				char *kvm_argv[1] = {class_name};
				int kvm_argc = 1;
				StartPVM(kvm_argc, kvm_argv);
			}
		}

		close(class_file);
	}

	LCD_Refresh();

	float f_a = 3.14159f;
	float f_b = 2.71828f;
	float f_c = f_a + f_b;
	float f_d = f_a - f_b;
	float f_e = f_a * f_b;
	float f_f = f_a / f_b;

	Debug_Printf(0, 5, false, 0, "a + b = %f\n", f_c);
	Debug_Printf(0, 6, false, 0, "a - b = %f\n", f_d);
	Debug_Printf(0, 7, false, 0, "a * b = %f\n", f_e);
	Debug_Printf(0, 8, false, 0, "a / b = %f\n", f_f);

	LCD_Refresh();

	// Example for getKey
	while (true)
	{
		uint32_t key1, key2;  // First create variables
		getKey(&key1, &key2); // then read the keys
		if (testKey(key1, key2, KEY_CLEAR))
		{ // Use testKey() to test if a specific key is pressed
			break;
		}
	}

	calcEnd(); // restore screen and do stuff
}