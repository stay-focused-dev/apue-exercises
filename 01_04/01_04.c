/* Если предположить, что календарное время хранится в виде 32-разрядного
целого числа со знаком, в каком году наступит переполнение? Какими
способами можно отдалить дату переполнения? Будут ли найденные решения
совместимы с существующими приложениями? */

#include "apue.h"
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define BYTE_SIZE 8

int max_year(time_t max_time_t) {
    /* struct tm имеет тип int для tm_year,
     * поэтому прикинем, не произойдет ли переполнение
     */
    struct tm max_year;
    memset(&max_year, 0, sizeof(max_year));
    max_year.tm_year = INT_MAX;
    time_t max_year_timestamp = mktime(&max_year);
    if (max_year_timestamp == -1) {
        err_quit("Unable to mktime");
    }

    if (max_time_t > max_year_timestamp) {
        return max_year.tm_year;
    }

    struct tm* t;
    t = localtime(&max_time_t);
    return 1900 + t->tm_year;
}

/* Когда time_t имел представление в виде int32, тогда существовала
 * так называемая проблема 2038 года. Отдалить эту дату можно было бы
 *   - поменяв signed тип на unsigned
 *   - поменяв int32 на int64
 *
 * Оба способа приводят к проблемам. Если time_t был знаковым, а стал
 * беззнаковым, то возможны проблемы с отрицательными значениями (они
 * могли представлять даты до начала времен). Если поменять тип с 
 * 32-битного на 64-битной, то поломается бинарная соместимость программ,
 * а также то, как хранится время в некоторых программах.
 */

int main(int argc, char *argv[]) {
    printf("max_year for signed int (32bit) = %d\n", max_year(INT_MAX));
    printf("max_year for unsigned int (32bit) = %d\n", max_year(UINT_MAX));
    printf("max_year for signed int (64bit) = %d\n", max_year(LLONG_MAX));

    int is_signed = (time_t) 0 > (time_t) -1;

    /* здесь предполагается, что:
     *     размер байта - 8 бит
     *     старший бит отвечает за знак
     */
    unsigned int sizeof_time_t = sizeof(time_t);
    time_t max_time_t = is_signed ? 0x7f : 0xff;
    int i;
    for (i = 1; i < sizeof_time_t; i++) {
        max_time_t = (max_time_t << BYTE_SIZE) + 0xff;
    }

    printf("time_t type is %s int (%dbit); max_year = %d\n",
            is_signed ? "signed" : "unsigned",
            BYTE_SIZE * sizeof_time_t,
            max_year(max_time_t));

    exit(0);
}
