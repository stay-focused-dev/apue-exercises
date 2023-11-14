/* Напишите свою версию функции dup2, которая реализует функциональность,
 * описанную в разделе 3.12, но без использования функции fcntl.
 * Предусмотрите обработку ошибок
 */

/* Описание из раздела 3.12:
 * Функция dup гарантирует, что возвращаемый ею новый файловый дескриптор
 * будет иметь наименьшее возможное значение. Вызывая функцию dup2, мы
 * указываем значение нового дескриптора в аргументе fd2. Если дескриптор
 * fd2 перед вызовом функции уже был открыт, он предварительно закрывается.
 * Если значения аргументов fd и fd2 равны, функция dup2 вернет дескриптор fd2,
 * не закрывая его. В противном случае для дескриптора fd2 сбрасывается флаг
 * FD_CLOEXEC, чтобы он оставлся открытым после вызова exec процессом.
 */
#include "apue.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

#define OPEN_MAX_GUESS 10240

long open_max(void) {
    if (openmax == 0) {
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            openmax = OPEN_MAX_GUESS;
        } else {
            err_sys("ошибка вызова sysconf с параметром _SC_OPEN_MAX");
        }
    }

    return(openmax);
}

int mydup2(int fd, int fd2) {
    if (fd < 0) {
        err_sys("первый аргумент mydup2 не может быть отрицательным");
    }

    if (fd2 < 0) {
        err_sys("второй аргумент mydup2 не может быть отрицательным");
    }

    if (fd == fd2) {
        return fd2;
    }
    /* man 2 close
     *
     * RETURN VALUES
     * Upon successful completion, a value of 0 is returned.
     * Otherwise, a value of -1 is returned and the global integer
     * variable errno is set to indicate the error.
     */
    if (close(fd2) == -1) {
        if (errno != EBADF) {
            err_sys("ошибка при вызове close для дескриптора %d", fd2);
        }
    }

    int *stack_ptr;
    int stack_pos;

    stack_ptr = (int*)malloc(open_max() * sizeof(int));
    if (stack_ptr == NULL) {
        err_sys("ошибка при вызове malloc");
    }
    stack_pos = 0;

    /* man 2 dup
     * RETURN VALUES
     * Upon successful completion, the new file descriptor is returned.
     * Otherwise, a value of -1 is returned and the global integer variable errno is set to indicate the error.
     */
    int val;
    while (1) {
        if ((val = dup(fd)) == -1) {
            free(stack_ptr);
            err_sys("ошибка при вызове dup");
        }

        if (val >= fd2) {
            break;
        }

        stack_ptr[stack_pos] = val;
        stack_pos++;
    }

    int i;
    for (i = 0; i < stack_pos && stack_ptr[stack_pos] != fd2; i++) {
        if (close(stack_ptr[i]) == -1) {
            free(stack_ptr);
            err_sys("ошибка при вызове close для дескриптора [%d] %d", i, stack_ptr[i]);
        }
    }
    free(stack_ptr);

    return val;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        err_quit("Использование: ./03_02 dup2|mydup2");
    }

    int fd2 = STDERR_FILENO;
    int fd3;

    char arg1_dup2[] = "dup2";
    char arg1_mydup2[] = "mydup2";

    if (strncmp(argv[1], arg1_dup2, sizeof(arg1_dup2)) == 0) {
        fd3 = dup2(fd2, 10);
    } else if (strncmp(argv[1], arg1_mydup2, sizeof(arg1_mydup2)) == 0) {
        fd3 = mydup2(fd2, 10);
    } else {
        err_quit("Использование: ./03_02 dup2|mydup2");
    }

    printf("pid: %d\n", getpid());

    char msg2[] = "Данные, записанные в fd2\n";
    char msg3[] = "Данные, записанные в fd3\n";

    ssize_t msg2size = sizeof(msg2);
    ssize_t msg3size = sizeof(msg3);

    ssize_t written;
    if ((written = write(fd2, msg2, msg2size)) != msg2size) {
        err_sys("не удалось записать msg2");
    }

    if ((written = write(fd3, msg3, msg3size)) != msg3size) {
        err_sys("не удалось записать msg3");
    }

    /* Можно использовать на MacOS команду:
     * lsof -p <PID>
     */
    usleep(10 * 1000000);

    exit(0);
}
