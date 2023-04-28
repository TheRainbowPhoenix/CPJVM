#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/file.hpp>

#include <string.h>
#include <stdlib.h>

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("Simple Calculator")
APP_DESCRIPTION("Guess what ? A calculator on a calculator !")
APP_AUTHOR("PC.")
APP_VERSION("0.0.1")

#define BUFFER_SIZE 256

int calc() {
    int sum = 0;
	int step = 0;

    char buffer[BUFFER_SIZE] = "32+8+2"; // Our math calculus

    char* token = strtok(buffer, "+-");
    while (token != NULL) {
        int number = atoi(token);

        if (strchr(token, '+') != NULL) {
            sum += number;
        } else if (strchr(token, '-') != NULL) {
            sum -= number;
        } else {
			sum += number;
	        Debug_Printf(0, step, false, 0,"input: '%s', number: %d\n", token, number);
            LCD_Refresh();
			step++;
        }

        token = strtok(NULL, "+-");
    }

    Debug_Printf(0, step, false, 0,"Result: %d\n", sum);
    LCD_Refresh();

    return 1;
}

extern "C"
void main() {
	calcInit(); //backup screen and init some variables

	// Put your app's code here!

	//Example for fillScreen(color);
	fillScreen(color(32,32,32));

    
    calc();

	//Example for getKey
	while(true){
		uint32_t key1, key2;	//First create variables
		getKey(&key1, &key2);	//then read the keys
		if(testKey(key1, key2, KEY_CLEAR)){ //Use testKey() to test if a specific key is pressed
			break;
		}
	}

	calcEnd(); //restore screen and do stuff
}