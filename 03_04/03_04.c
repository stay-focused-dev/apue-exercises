#include "apue.h"
#include <fcntl.h>

int main(int argc, char * argv[]) {
    int val;

    if (argc != 2) {
        err_quit("Использование: ./03_04 <номер дескриптора>");
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
        err_sys("ошибка fcntl для дескриптора %d", atoi(argv[1]));
    }

    switch (val & O_ACCMODE) {
        case O_RDONLY:
            printf("только для чтения");
            break;

        case O_WRONLY:
            printf("только для записи");
            break;

        case O_RDWR:
            printf("для чтения и для записи");
            break;

        default:
            err_dump("неизвестный режим доступа");
    }

    if (val & O_APPEND) {
        printf(", добавление в конец");
    }
    if (val & O_NONBLOCK) {
        printf(", неблокирующий режим");
    }
    if (val & O_SYNC) {
        printf(", синхронный режим записи");
    }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC) {
        printf(", синхронный режим записи");
    }
#endif

    putchar('\n');
    exit(0);
}
