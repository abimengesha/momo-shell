//#include "user.h"

/*int
main(void) {
    // try to call the shutdown system call
    if (shutdown() == 0) {
        return 0;
    } else {
        return 1; //if fails return 1
    }
}*/

#include "kernel/types.h"
#include "user/user.h"

int
main(void) {
  shutdown();
  return 0;
}
