/*#include "user.h"  // Assuming your user-space header
#include "kernel/types.h"


int main() {
    // Call the strace_on system call to enable tracing
    strace_on();  // This should enable tracing for the current process

    // Make some system calls to test the tracing
    printf("System call tracing enabled\n");
    printf("PID: %d\n", getpid());  // Should trace the sys_getpid system call
    exit(0);  // Should trace the sys_exit system call

    return 0;
}*/
