/*
 * Измените программу из листинга 2.4 так, чтобы избежать лишней работы, когда функция sysconf
 * возвращает значение LONG_MAX для предела OPEN_MAX
 */


/* листинг 2.4

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
*/

#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

long open_max(void) {
    if (openmax == 0) {
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            if (errno == 0) {
                openmax = LONG_MAX;
            } else {
                err_sys("ошибка вызова sysconf с параметром _SC_OPEN_MAX");
            }
        }

        /*
         * Цитата из книги:
         * Но если сделать верхний предел практически неограниченным,
         * функция sysconf будет возвращать LONG_MAX в качестве предела
         * OPEN_MAX. Тогда программа, которая ориентируется на значение
         * верхнего предела, будет затрачивать огромное количество времени
         * на попытки закрыть LONG_MAX дескрипторов, большинство из которых
         * даже не открывались.
         *
         * Системы, поддерживающие расширение XSI стандарта SUS, предоставляют
         * функцию getrlimit. Они может использоваться для получения максимального
         * количества открытых дескрипторов на процесс.
         */

        /* man getrlimit:
         *
         * int getrlimit(int resource, struct rlimit *rlp);
         *
         *      The resource parameter is one of the following:
         *
         *      RLIMIT_NOFILE   The maximum number of open files for this process.
         *      ...
         *
         *      RETURN VALUES
         *      A 0 return value indicates that the call succeeded, changing or returning the resource limit.
         *      A return value of -1 indicates that an error occurred,
         *          and an error code is stored in the global location errno.
         */
        if (openmax == LONG_MAX) {
            struct rlimit rlp = {};
            if (getrlimit(RLIMIT_NOFILE, &rlp) == -1) {
                err_sys("ошибка вызова getrlimit с параметром RLIMIT_NOFILE");
            }
            openmax = rlp.rlim_max;
        }
    }

    return(openmax);
}

int main(int argc, char * argv[]) {
    long openmax = open_max();
    printf("openmax = %ld\n", openmax);
    exit(0);
}
