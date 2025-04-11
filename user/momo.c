#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h" 


#define MAXARGS 10 //max num of args
#define MAXBUF 128 //max buffer for input
#define MAX_HISTORY 100  // max num of stored commands

//Global variables
char *history[MAX_HISTORY];  //array for History command storage
int history_count = 0;  //num  of commands stored
int last_exit_status = 0; // last command exit status

// Function for  strcat
char *strcat(char *dest, const char *src) {
    char *ptr = dest + strlen(dest); //move ptr to the end of dest
    while (*src) {
        *ptr++ = *src++; //copy each character from src to dest
    }
    *ptr = '\0'; //null-terminate the result
    return dest;
}


// Function to find length of prefix of chars
uint strspn(const char *str, const char *chars) {
    uint i, j;
    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; chars[j] != str[i]; j++) {
            if (chars[j] == '\0')
                return i;
        }
    }
    return i;
}

// function to find first occurance
uint strcspn(const char *str, const char *chars) {
    const char *p, *sp;
    char c, sc;
    for (p = str;;) {
        c = *p++;
        sp = chars;
        do {
            if ((sc = *sp++) == c) {
                return (p - 1 - str);
            }
        } while (sc != 0);
    }
}

// function for dupllicating a string
char* strdup(const char* s) {
    char* d = malloc(strlen(s) + 1); //allocate mem for new string
    if (d == NULL) return NULL;
    strcpy(d, s); //copy contents of s into the new space
    return d;
}

// gets next token from a string
char *next_token(char **str_ptr, const char *delim) {
    if (*str_ptr == NULL) {
        return NULL;
    }

    uint tok_start = strspn(*str_ptr, delim);
    uint tok_end = strcspn(*str_ptr + tok_start, delim);

    if (tok_end == 0) {
        *str_ptr = NULL;
        return NULL;
    }

    char *current_ptr = *str_ptr + tok_start;
    *str_ptr += tok_start + tok_end;

    if (**str_ptr == '\0') {
        *str_ptr = NULL;
    } else {
        **str_ptr = '\0';
        (*str_ptr)++;
    }

    return current_ptr;
}

// stores commands in history
void add_history(char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);  // remove oldest command
        for (int i = 1; i < MAX_HISTORY; i++) {
            history[i - 1] = history[i];  // shift history left
        }
        history[MAX_HISTORY - 1] = strdup(cmd);
    }
}

// running the command given
int runcmd(char *buf) {
    char *argv[MAXARGS]; //array for command args
    int argc = 0;
    char *next_tok = buf;
    char *curr_tok;

    // ignore everything after # comment symbol
    char *comment_pos = strchr(buf, '#');
    if (comment_pos) {
        *comment_pos = '\0';  // null-terminate before #
    }

    // tokenize input into arguments
    while ((curr_tok = next_token(&next_tok, " \t\r\n\v")) != NULL && argc < MAXARGS - 1) {
        argv[argc++] = curr_tok;
    }
    argv[argc] = 0; // null-terminate argv

    if (argc == 0) { //empty command  so do nothing
        return 0;  
    }

    // Store command in history
    if (history_count < MAX_HISTORY) {
 		add_history(buf);
    }

    // Built-in commands
    if (strcmp(argv[0], "cd") == 0) {
       	if (argc < 2) {
            printf("momo: cd missing argument\n");
        } else if (chdir(argv[1]) < 0) {
            printf("momo: cannot cd %s\n", argv[1]);
        } 
        return 1;
    }

	//exit command
    if (strcmp(argv[0], "exit") == 0) {
    	printf("It's offical we're out :0");
        exit(0);
    }

    // pwd command
    if (strcmp(argv[0], "pwd") == 0) {
        char path[MAXPATH] = "";
        if (getcwd(path, MAXPATH) < 0)
            printf("momo: getcwd failed\n");
        else
            printf("%s\n", path);
        return 0;
    }

    // display command history 
    if (strcmp(argv[0], "history") == 0) {
        for (int i = 0; i < history_count; i++) {
            printf("%d %s\n", i + 1, history[i]);
        }
        return 0;
    }

    // External command execution
    if (fork() == 0) {	    
	    exec(argv[0], argv);  // Try executing as given

	    //try absolute path in /
	    char path[MAXBUF];
	    strcpy(path, "/");
	    strcat(path, argv[0]);
	    exec(path, argv);

	    //try executing from curr directory 
	    strcpy(path, "./");
	    strcat(path, argv[0]);
	    exec(path, argv);

		// if execution fails
	    printf("momo: exec %s failed\n", argv[0]);  // Print error if all fail
	    exit(1);
    } else {
        int status;
        wait(&status);
        last_exit_status = status >> 8;
    }
    return 0;
}


// Print the shell prompt
void print_prompt(int cmd_num) {
	char temp_cwd[MAXPATH]; // temp storage for getcwd

    if (!getcwd(temp_cwd, sizeof(temp_cwd))) {
        strcpy(temp_cwd, "/");  // set default to root if getcwd fails
    }    
    
    printf("[%d]-[%d]─[%s] momo$ ", last_exit_status, cmd_num, temp_cwd);
}


// Execute commands from a script file
void runscript(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("momo: cannot open %s\n", filename);
        exit(1);
    }

    char buf[MAXBUF];
    int n;
    char *line, *next_line;

	//read and process script file
    while ((n = read(fd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';  // null-terminate the buffer
        next_line = buf;

        // process each line in the script
        while ((line = next_token(&next_line, "\n")) != NULL) {
            if (*line != '\0') {  // ignore empty lines
                runcmd(line);  // run each command
            }
        }
    }

    close(fd);
    exit(0);
}



// main function handles user input and starts shell 
int main(int argc, char *argv[]) {
    static char buf[MAXBUF];
    int cmd_num = 0;

    if (argc > 1) {
        runscript(argv[1]); // Scripting mode
    }

    //shell mode
    while (1) {
        print_prompt(++cmd_num);  // display the shell prompt
        memset(buf, 0, sizeof(buf));  // clear input buffer
        read(0, buf, sizeof(buf));  // Get user input

        if (buf[0] == 0) {
            continue;  // ignore empty input
        }

        // execute the command and check if the prompt should update immediately
        int result = runcmd(buf);
        if (result == 1) {
            continue;  // Update the prompt after cd
        }
    }

    return 0;
}
