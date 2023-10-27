#include "apue.h"
#include <errno.h>
#include <limits.h>

static void pr_sysconf(char *, int);
static void pr_pathconf(char *, char *, int);

int main(int argc, char * argv[]) {
    if (argc != 2) {
        err_quit("Использование: 02_02 <каталог>");
    }

#ifdef ARG_MAX
    printf("ARG_MAX определен как %d\n", ARG_MAX+0);
#else
    printf("идентификатор ARG_MAX не найден\n");
#endif
#ifdef _SC_ARG_MAX
    pr_sysconf("ARG_MAX = ", _SC_ARG_MAX);
#else
    printf("идентификатор _SC_ARG_MAX не найден\n");
#endif

#ifdef NAME_MAX
    printf("NAME_MAX определен как %d\n", NAME_MAX+0);
#else
    printf("идентификатор NAME_MAX не найден\n");
#endif
#ifdef _PC_NAME_MAX
    pr_pathconf("NAME_MAX = ", argv[1], _PC_NAME_MAX);
#else
    printf("идентификатор _PC_NAME_MAX не найден\n");
#endif

    exit(0);
}

static void pr_sysconf(char *mesg, int name) {
    long val;

    fputs(mesg, stdout);
    errno = 0;
/* If the call to sysconf() is not successful, -1 is returned and errno is set appropriately.
 * Otherwise, if the variable is associated with functionality that is not supported, -1 is returned and errno is not modified.
 * Otherwise, the current variable value is returned.
 *
 * [EINVAL] The value of the name argument is invalid.*/
    if ((val = sysconf(name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                fputs(" (не поддерживается)\n", stdout);
            } else {
                err_sys("ошибка вызова sys_conf");
            }
        } else {
            fputs(" (нет ограничений)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}

static void pr_pathconf(char *mesg, char *path, int name) {
    long val;

    fputs(mesg, stdout);
    errno = 0;
    /* If the call to pathconf or fpathconf is not successful, -1 is returned and errno is set appropriately.
     * Otherwise, if the variable is associated with functionality that does not have a limit in the system, -1 is returned and errno is not modified.
     * Otherwise, the current variable value is returned
     *
     * [EINVAL] The implementation does not support an association of the variable name with the associated file. */
    if ((val = pathconf(path, name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                fputs(" (не поддерживается)\n", stdout);
            } else {
                err_sys("ошибка вызова path_conf, path = %s", path);
            }
        } else {
            fputs(" (нет ограничений)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}
