/**
 * @file broken.c
 * @author mmalensek
 *
 * This program contains a series of buggy, broken, or strange C functions for
 * you to ponder. Your job is to analyze each function, fix whatever bugs the
 * functions might have, and then explain what went wrong. Sometimes the
 * compiler will give you a hint.
 *
 *  ____________
 * < Good luck! >
 *  ------------
 *      \   ^__^
 *       \  (oo)\_______
 *          (__)\       )\/\
 *              ||----w |
 *              ||     ||
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


static int func_counter = 1;
#define FUNC_START() printf("\n\n%d.) %s\n", func_counter++, __func__);


/**
 * This awesome code example was taken from the book 'Mastering C Pointers,'
 * one of the famously bad resources on learning C. It was trying to demonstrate
 * how to print 'BNGULAR'... with pointers...? Maybe?
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:
 *      Hint: where are string literals stored in memory?
 *      Hint: what is '66' in this example? Can we do better?

 		So originally the code was trying to change the string, but I changed it by using a mutable char array.
 		Then I modified the word using *a = 'B', to change the string from 'ANGULAR' to 'BNGULAR' since "a" is now
 		a mutable char array.
 */
void
angular(void)
{
  FUNC_START();

  char a[] = "ANGULAR";
  *a = 'B';
  printf("%s\n", a);
}

/**
 * This function is the next step after 'Hello world' -- it takes user input and
 * prints it back out! (Wow).
 *
 * But, unfortunately, it doesn't work.
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:

 	So, char *name = 0; is an uninitialzed pointer, so I used an array, char name[128], which allocated 128 bytes of
 	memory on stack.
 *
 *   (answer)
 */
void
greeter(void)
{
  FUNC_START();
  
  char name[128]; 

  printf("Please enter your name: ");
  gets(name, 128);

  // Remove newline character
  char *p = name;
  for ( ; *p != '\n' && *p != 0; p++) { }
  *p = '\0';

  printf("Hello, %s!\n", name);

}

/**
 * This 'useful' function prints out an array of integers with their indexes, or
 * at least tries to. It even has a special feature where it adds '12' to the
 * array.
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:

	 I changed 14[stuff + 1] = 12 to stuff[15] = 12; so its easier to read.
	 Then I corrected the loop to have 100 iterations instead of 1000.
	 
 *
 *   (answer)
 */
void
displayer(void)
{
  FUNC_START();
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Waggressive-loop-optimizations"

  int stuff[100] = { 0 };

  /* Can you guess what the following does without running the program? */
  /* Rewrite it so it's easier to read. */
  //14[stuff + 1] = 12;
  stuff[15] = 12;

  for (int i = 0; i < 100; ++i) {
    printf("%d: %d\n", i, stuff[i]);
  }

  #pragma GCC diagnostic pop
}

/**
 * Adds up the contents of an array and prints the total. Unfortunately the
 * total is wrong! See main() for the arguments that were passed in.
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:
 *
 *   (answer)
 In the original code, it was using sizeof(arr) to know how many iterations to go through. 
 But arr was a pointer to the array not the actual array so it was counting only the size of the pointer
 not the elements of array. So passing int(size) as a parameter and adding it into the for loop
 along with passing it into the main function fixed the problem and is now correctly adding
 the sum.
 */

 
void
adder(int *arr, int size)
{
  FUNC_START();

  int total = 0;

  for (int i = 0; i < size; ++i) {
    total += arr[i];
  }

  printf("Total is: %d\n", total);
}

/**
 * This function is supposed to be somewhat like strcat, but it doesn't work.
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:
 *
 *   (answer)
 So, the local char array couldn't  be be accessed outside the function.
 So I had to allocate memory for the buffer using malloc, so that the array
 can be accessed outside the function.
 */
char *
suffixer(char *a, char *b)
{
  FUNC_START();
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wdangling-pointer"

  char *buf = malloc(strlen(a) + strlen(b) + 1);
  char *buf_start = buf;
  strcpy(buf, a);
  strcpy(buf + strlen(a), b);
  return buf_start;


  #pragma GCC diagnostic pop
}

/**
 * This is an excerpt of Elon Musk's Full Self Driving code. Unfortunately, it
 * keeps telling people to take the wrong turn. Figure out how to fix it, and
 * explain what's going on so Elon can get back to work on leaving Earth for
 * good.
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:
 *
 *   (answer)
 So the char array is was 8 which wasn't enough for storing the
 longer strings. But I made it size 16 array so that there is enough
 space to print the long strings.
 
 */
void
driver(void)
{
  FUNC_START();

  char street1[16] = { "fulton" };
  char street2[16] = { "gomery" };
  char street3[16] = { "piedmont" };
  char street4[16] = { "baker" };
  char street5[16] = { "haight" };

  if (strcmp(street1, street2)) {
    char *new_name = "saint agnew ";
    memcpy(street4, new_name, strlen(new_name));
  }

  printf("Welcome to TeslaOS 0.1!\n");
  printf("Calculating route...\n");
  printf("Turn left at the intersection of %s and %s.\n", street5, street3);
}

/**
 * This function tokenizes a string by space, sort of like a basic strtok or
 * strsep. It has two subtle memory bugs for you to find.
 *
 * (1) Fix the problem.
 * (2) Explain what's wrong with this code:
 *
 *   (answer)
 I allocated an extra byte so their won't be a memory leak.
 */
void
tokenizer(void)
{
  FUNC_START();

  char *str = "Hope was a letter I never could send";
  char *line = malloc(strlen(str) + 1);
  char *c = line;

  char *nline = line;

  strcpy(line, str);

  while (*c != '\0') {

    for ( ; *c != ' ' && *c != '\0'; c++) {
      // find the next space (or end of string)
    }

    *c = '\0';
    printf("%s\n", line);

    line = c + 1;
    c = line;
  }

  free(nline);
}

/**
* This function should print one thing you like about C, and one thing you
* dislike about it. Assuming you don't mess up, there will be no bugs to find
* here!
*/
void
finisher(void)
{
  FUNC_START();
	printf("One thing that I like about C is the it's efficent.\n");
	printf("One thing I don't like about C is there's too many \nseg faults and at times I cannot find \nmy errors as easily as other languages.\n");
}

int
main(void)
{
  printf("Starting up!");

  angular();

  greeter();

  displayer();

  int nums[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };
  adder(nums,10);//pass size of array 

  char *result = suffixer("kayak", "ing");
  printf("Suffixed: %s\n", result);

  driver();

  tokenizer();

  finisher();

  return 0;
}
