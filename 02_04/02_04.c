#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

#define OPEN_MAX_GUESS 256

long open_max(void) {
    if (openmax == 0) {
        errno = 0;
        /*
         * If the call to sysconf() is not successful, -1 is returned and errno is set appropriately.
         * Otherwise, if the variable is associated with functionality that is not supported, -1 is returned and errno is not modified.
         * Otherwise, the current variable value is returned.
         */
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            if (errno == 0) {
                openmax = OPEN_MAX_GUESS;
            } else {
                err_sys("ошибка вызова sysconf с параметром _SC_OPEN_MAX");
            }
        }
    }

    return(openmax);
}

int main(int argc, char * argv[]) {
    long openmax = open_max();
    printf("openmax = %ld\n", openmax);
    exit(0);
}
