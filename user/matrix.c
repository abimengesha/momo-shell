#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// CITATION : CHATGPT walked us through how to do custom random num generator since we cant use rand()
// random num generator to maintain curr seed for the RNG
unsigned int seed = 12345678;

//linear congruential generator algorithm to generate random number 
// CITATION: got code from https://stackoverflow.com/questions/24494611/linear-congruential-generator-c
//method to generate random number bc we can't use rand()
unsigned int rand_lcg() {
    seed = (1103515245 * seed + 12345) & 0x7fffffff;
    return seed;
}

//return a random number from 0 to "max" using the rand-lcg function
unsigned int rand_range(int max) {
    return rand_lcg() % max;
}

// function to clear the screen using ANSI escape code
void clear_screen() {
    printf("\033[H\033[J");
}

// Function to generate matrix rain effect
void matrix_effect(int rows, int cols, int speed, int duration, const char *color, int rainbow) {
	// characters used in the matrix rain effect
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    // len  of the charset arr /wo null termminator  used to select random characters
    int charset_len = sizeof(charset) - 1;

    // color array for rainbow flag
    const char* colors[] = {
        "\033[31m", // red
        "\033[33m", // y3llow :>
        "\033[32m", // green
        "\033[34m", // blue
        "\033[35m", // magenta
        "\033[36m", // cyan
    };
    
    int num_colors = sizeof(colors) / sizeof(colors[0]);
    int time_elapsed = 0; // counter for how long the effect will run
    int column_pos[cols];  // track positions for each column's falling chars

    // initialize each column to a random starting row
    for (int j = 0; j < cols; j++) {
        column_pos[j] = rand_range(rows);
    }
    // loop to run the effect for the duration (set iteration of 2)
    while (time_elapsed < duration) {
        clear_screen();  // clear the screen at each iteration

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
            //for each column and row  in matrix: check if rainbow is set and get color
                const char* current_color = (rainbow ? colors[j % num_colors] : color);
                //make sure row and col are equal in order to print
                if (i == column_pos[j]) {
                	//get random number to print
                    char ch = charset[rand_range(charset_len)];
                    printf("%s%c\033[0m", current_color, ch);
                    //print char in a diff postiion to stagger output
                } else if (rand_range(10) < 2) {
                    printf("%s%c\033[0m", current_color, charset[rand_range(charset_len)]);
                } else {
                //if all else fails just print a space
                    printf(" ");
                }
            }
            printf("\n");  // new line after each row
        }

        //Update the position of the falling streams
        for (int j = 0; j < cols; j++) {
            column_pos[j] = (column_pos[j] + 1) % rows;
        }

        sleep(speed);  // Adjust speed for the matrix falling effect
        time_elapsed += speed;  // Keep track of time elapsed
    }

}

int main(int argc, char *argv[]) {
    int rows = 80; // 80 by 24 for columns and rows
    int cols = 24;  
    int speed = 1;  //  speed
    int duration = 2;  // iterations
    int rainbow = 0;  // flag for rainbow effect

    const char *color = "\033[32m";  // default green color
    // conditional for checking flags
    if (argc > 1) { 
        if (strcmp(argv[1], "-pink") == 0) {
            color = "\033[95m";  // pink 
        } else if (strcmp(argv[1], "-blue") == 0) {
            color = "\033[94m";  // blue 
        } else if(strcmp(argv[1], "-rainbow") == 0){
        	rainbow = 1;  // enable rainbow effect
        }
        
    }
    printf("Color code set to: %s\n", color);

    clear_screen();  // clear screen b4 starting effect
    matrix_effect(rows, cols, speed, duration, color, rainbow);  // call the matrix effect

    return 0;
}

