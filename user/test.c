#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

/* CITATION:!!!!!!!!!!!!!
My test cases weren't working so I asked ChatGPT
to help me with my test cases!

CITATION!!!!!!!

*/

void test_fgets(int fd) {
    char buffer[100];
    int numOfBytes = fgets(fd, buffer, sizeof(buffer));
    if (numOfBytes < 0) {
        printf("fgets failed or reached EOF.\n");
    } else {
        printf("Read %d characters from fgets: %s",numOfBytes, buffer);
    }
}

void test_getline(int fd) {
    char *buffer = NULL;
    int length = getline(&buffer, fd);
    if (length < 0) {
        printf("getline failed or reached EOF.\n");
    } else {
        printf("Read %d bytes from getline: %s",length, buffer);
    }
    free(buffer);  // Free the dynamically allocated buffer
}

int main() {
    int fd = open("testfile.txt", O_RDONLY);
    if (fd < 0) {
        printf("Error opening file.\n");
        exit(1);
    }

    printf("Testing fgets():\n");
    test_fgets(fd);
    close(fd);

    fd = open("testfile.txt", O_RDONLY);
    printf("\nTesting getline():\n");
    test_getline(fd);

    close(fd);
    exit(0);
}

