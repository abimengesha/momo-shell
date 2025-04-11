#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// test function for malloc and free
void test_malloc() {
    printf("\n===== TEST: Basic malloc and free =====\n");

    char *p1 = (char *) malloc(32);
    printf("Allocated 32 bytes at %p\n", p1);

    char *p2 = (char *) malloc(64);
    printf("Allocated 64 bytes at %p\n", p2);

    free(p1);
    printf("Freed 32-byte block at %p\n", p1);

    free(p2);
    printf("Freed 64-byte block at %p\n", p2);

    malloc_print();
}


// test function for calloc
void test_calloc() {
    printf("\n===== TEST: calloc =====\n");

    int *arr = (int *) calloc(4, sizeof(int));
    if (arr == NULL) {
        printf("calloc failed!\n");
        return;
    }
    printf("Allocated array with 4 integers at %p\n", arr);

    for (int i = 0; i < 4; i++) {
        if (arr[i] != 0) {
            printf("calloc failed to zero out memory!\n");
            return;
        }
    }
    printf("calloc correctly zeroed out memory.\n");

    free(arr);
    printf("Freed allocated array.\n");
}

// test function for malloc_print
void test_malloc_print() {
    printf("\n===== TEST: malloc_print =====\n");

    char *p1 = (char *) malloc(16);
    char *p2 = (char *) malloc(32);
    char *p3 = (char *) malloc(64);

    printf("Allocated memory blocks.\n");
    malloc_print();  //print the state of memory

    free(p1);
    free(p2);
    free(p3);

    printf("Freed all memory blocks.\n");
    malloc_print();  //print memory state again
}



int main() {
    test_malloc();
    test_calloc();
    test_malloc_print();
    exit(0);
}

