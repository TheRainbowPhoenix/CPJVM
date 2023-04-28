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

int     calculate(char* expression, int* pos);
int     evaluate(char* expression);
char*   strip(char* buffer);

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

/**
 * Strip any space from the buffer, by copying non-space one per one
*/
char* strip(char* buffer) {
    int len = strlen(buffer);
    int i, j;
    for (i = 0, j = 0; i < len; i++) {
        if (buffer[i] != ' ') {
            buffer[j] = buffer[i];
            j++;
        }
    }
    buffer[j] = '\0';
}

int evaluate(char* expression) {
    int pos = 0;
    return calculate(expression, &pos);
}

int calculate(char* expression, int* pos) {
    int sum = 0;
    int current_num = 0;
    char current_op = '+';

    while (expression[*pos] != '\0') {
        if (expression[*pos] >= '0' && expression[*pos] <= '9') {
            current_num = current_num * 10 + (expression[*pos] - '0');
        } else if (expression[*pos] == '+' || expression[*pos] == '-') {
            switch (current_op) {
                case '+':
                    sum += current_num;
                    break;
                case '-':
                    sum -= current_num;
                    break;
            }
            current_num = 0;
            current_op = expression[*pos];
        }
        (*pos)++;
    }

    switch (current_op) {
        case '+':
            sum += current_num;
            break;
        case '-':
            sum -= current_num;
            break;
    }

    return sum;
}

extern "C"
void main() {
	calcInit(); //backup screen and init some variables

	// Put your app's code here!

	//Example for fillScreen(color);
	fillScreen(color(32,32,32));

    char buffer[BUFFER_SIZE] = "32 + 8 + 2"; // Our math calculus

    int result = evaluate(strip(buffer));

    Debug_Printf(0, step, false, 0,"Result: %d\n", result);

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