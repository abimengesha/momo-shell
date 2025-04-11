/*#include "user.h"

int
main(void) {
    // try to call the shutdown system call
    if (reboot() == 0) {
        return 0;
    } else {
        return 1; //if fails return 1
    }
}*/

#include "kernel/types.h"
#include "user/user.h"

int
main(void) {
  reboot();
  return 0;
}
