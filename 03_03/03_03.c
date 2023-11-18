#include "apue.h"
#include <fcntl.h>

void set_fd(int fd, int flags) {
    int val;
    if ((val = fcntl(fd, F_GETFD, 0)) < 0) {
        err_sys("ошибка выполнения команды F_GETFD функции fcntl");
    }

    val |= flags;

    if (fcntl(fd, F_SETFD, val) < 0) {
        err_sys("ошибка выполнения команды F_SETFD функции fcntl");
    }
}

void clr_fd(int fd, int flags) {
    int val;
    if ((val = fcntl(fd, F_GETFD, 0)) < 0) {
        err_sys("ошибка выполнения команды F_GETFD функции fcntl");
    }

    val &= ~flags;

    if (fcntl(fd, F_SETFD, val) < 0) {
        err_sys("ошибка выполнения команды F_SETFD функции fcntl");
    }
}

int is_set_fd(int fd, int flag) {
    int val;
    if ((val = fcntl(fd, F_GETFD, 0)) < 0) {
        err_sys("ошибка выполнения команды F_GETFD функции fcntl");
    }

    return val & flag ? 1 : 0;
}

void set_fl(int fd, int flags) {
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("ошибка выполнения команды F_GETFL функции fcntl");
    }

    val |= flags;

    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("ошибка выполнения команды F_SETFL функции fcntl");
    }
}

void clr_fl(int fd, int flags) {
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("ошибка выполнения команды F_GETFL функции fcntl");
    }

    val &= ~flags;

    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("ошибка выполнения команды F_SETFL функции fcntl");
    }
}

int is_set_fl(int fd, int flag) {
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("ошибка выполнения команды F_GETFL функции fcntl");
    }

    return val & flag ? 1 : 0;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        err_quit("Использование: ./03_03 <путь к файлу>");
    }

    int oflags = O_RDWR | O_CREAT | O_TRUNC;

    int fd1 = open(argv[1], oflags);
    int fd2 = dup(fd1);
    int fd3 = open(argv[1], oflags);

    /* проверяем, на какие дескрипторы окажет влияние fcntl,
     * если передаь ей в качестве аргументов fd1 и F_SETFD
     */
    printf("Проверяем влияние fcntl с F_SETFD:\n");
    int fd_flag = FD_CLOEXEC;
    int is_set_fd_flag_fd1 = is_set_fd(fd1, fd_flag);
    int is_set_fd_flag_fd2 = is_set_fd(fd2, fd_flag);
    int is_set_fd_flag_fd3 = is_set_fd(fd3, fd_flag);
    if (is_set_fd_flag_fd1) {
        clr_fd(fd1, fd_flag);
    } else {
        set_fd(fd1, fd_flag);
    }

    (is_set_fd_flag_fd1 == is_set_fd(fd1, fd_flag))
        ? printf("не изменились флаги дескриптора fd1 при вызове fcnt с F_SETFD\n")
        : printf("изменились флаги дескриптора fd1 при вызове fcntl с F_SETFD\n");

    (is_set_fd_flag_fd2 == is_set_fd(fd2, fd_flag))
        ? printf("не изменились флаги дескриптора fd2 при вызове fcnt с F_SETFD\n")
        : printf("изменились флаги дескриптора fd2 при вызове fcntl с F_SETFD\n");

    (is_set_fd_flag_fd3 == is_set_fd(fd3, fd_flag))
        ? printf("не изменились флаги дескриптора fd3 при вызове fcnt с F_SETFD\n")
        : printf("изменились флаги дескриптора fd3 при вызове fcntl с F_SETFD\n");

    printf("\n");

    /* проверяем, на какие дескрипторы окажет влияние fcntl,
     * если передать ей в качестве аргументов fd1 и F_SETFL
     */
    printf("Проверяем влияние fcntl с F_SETFL:\n");

    int fl_flag = O_NONBLOCK;
    int is_set_fl_flag_fd1 = is_set_fl(fd1, fl_flag);
    int is_set_fl_flag_fd2 = is_set_fl(fd2, fl_flag);
    int is_set_fl_flag_fd3 = is_set_fl(fd3, fl_flag);
    if (is_set_fl_flag_fd1) {
        clr_fl(fd1, fl_flag);
    } else {
        set_fl(fd1, fl_flag);
    }

    (is_set_fl_flag_fd1 == is_set_fl(fd1, fl_flag))
        ? printf("не изменились флаги дескриптора fd1 при вызове fcnt с F_SETFL\n")
        : printf("изменились флаги дескриптора fd1 при вызове fcntl с F_SETFL\n");

    (is_set_fl_flag_fd2 == is_set_fl(fd2, fl_flag))
        ? printf("не изменились флаги дескриптора fd2 при вызове fcntl с F_SETFL\n")
        : printf("изменились флаги дескриптора fd2 при вызове fcntl с F_SETFL\n");

    (is_set_fl_flag_fd3 == is_set_fl(fd3, fl_flag))
        ? printf("не изменились флаги дескриптора fd3 при вызове fcntl с F_SETFL\n")
        : printf("изменились флаги дескриптора fd3 при вызове fcntl с F_SETFL\n");

    exit(0);
}
