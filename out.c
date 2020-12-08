#include <stdio.h>
#include <stdlib.h>

#include "main.h"

/* =====================================================================
 *
 * Length_number
 *
 * =====================================================================
 * функция которая считает длину числа
 *
 *  Вход:                               число
 *
 *  Выход:                              1   длина числа которое меньше 10
 *                                      >1  длина числа которое больше или равно 10
 */
int Length_number(int i)
{
    if(i < 10) return 1;

    int int_length = 0;

    while(i != 0)
    {
        int_length++;
        i = i / 10;
    }

    return int_length;
}

/* =====================================================================
 *
 * Out_statistic
 *
 * =====================================================================
 * функция которая выводит всю статистику в консоль
 *
 *  Вход:
 *    глобальные переменные:            stat_files_ext_t        структура которая хранит в себе переменные c количеством исходных файлов с разными расширениями
 *                                      stat_all_str_number_t   структура которая хранит в себе переменные c количеством строк в исходных файлах с разными расширениями
 *                                      stat_code_str_number_t  структура которая хранит в себе переменные c количеством строк кода в исходных файлах с разными расширениями
 *                                      int_files_total_scan    переменная хранящая число просканированных файлов
 *                                      int_dir_total_scan      переменная хранящая число просканированных директорий
 *
 *
 *  Выход:                              вывод всей статистики в консоль
 */
void Out_statistic()
{
    printf("\n\nScanned %d files, %d directories: ", int_files_total_scan, int_dir_total_scan);
    /* первая строка с разделением таблиц */
    printf("\n%c", 201);
    for(int k = 0; k < 63; k++)
    {
        if(k == 15) printf("%c", 203);
        if(k == 31) printf("%c", 203);
        if(k == 46) printf("%c", 203);
        printf("%c", 205);
    }
    printf("%c\n", 187);

    printf("%cFile extensions%c  Source files  %c     Lines     %c  Lines of Code  %c", 186, 186, 186, 186, 186);
    /* ================================== */

    /* разделение между таблицами и строками */
    printf("\n%c", 204);
    for(int k = 0; k < 63; k++)
    {
        if(k == 15) printf("%c", 206);
        if(k == 31) printf("%c", 206);
        if(k == 46) printf("%c", 206);
        printf("%c", 205);
    }
    printf("%c\n", 185);
    /* ===================================== */

    /* вывод статистики по .c файлам и строкам */
    for(int k = 0; k < 63; k++)
    {
        if(k == 0)
        {
            printf("%c .c", 186);
            k += 4;
        }

        printf(" ");
        if(k == 15)
        {
            long temp = stat_files_ext.c;
            printf("%c %d", 186, stat_files_ext.c);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 31)
        {
            long temp = stat_all_str_number.c;
            printf("%c %ld", 186, stat_all_str_number.c);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 46)
        {
            long temp = stat_code_str_number.c;
            printf("%c %ld", 186, stat_code_str_number.c);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }
    }
    printf(" %c\n", 186);
    /* ===================================== */


    /* вывод статистики по .cpp файлам и строкам */
    for(int k = 0; k < 63; k++)
    {
        if(k == 0)
        {
            printf("%c .cpp", 186);
            k += 6;
        }

        printf(" ");
        if(k == 15)
        {
            long temp = stat_files_ext.cpp;
            printf("%c %d", 186, stat_files_ext.cpp);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 31)
        {
            long temp = stat_all_str_number.cpp;
            printf("%c %ld", 186, stat_all_str_number.cpp);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 46)
        {
            long temp = stat_code_str_number.cpp;
            printf("%c %ld", 186, stat_code_str_number.cpp);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }
    }
    printf(" %c\n", 186);
    /* ================================================== */

    /* вывод статистики по .h файлам и строкам */
    for(int k = 0; k < 63; k++)
    {
        if(k == 0)
        {
            printf("%c .h", 186);
            k += 4;
        }

        printf(" ");
        if(k == 15)
        {
            long temp = stat_files_ext.h;
            printf("%c %d", 186, stat_files_ext.h);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 31)
        {
            long temp = stat_all_str_number.h;
            printf("%c %ld", 186, stat_all_str_number.h);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 46)
        {
            long temp = stat_code_str_number.h;
            printf("%c %ld", 186, stat_code_str_number.h);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }
    }
    printf(" %c\n", 186);
    /* ============================================ */

    /* вывод статистики по .as/asm файлам и строкам */
    for(int k = 0; k < 63; k++)
    {
        if(k == 0)
        {
            printf("%c .as", 186);
            k += 5;
        }

        printf(" ");
        if(k == 15)
        {
            long temp = stat_files_ext.as;
            printf("%c %d", 186, stat_files_ext.as);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 31)
        {
            long temp = stat_all_str_number.as;
            printf("%c %ld", 186, stat_all_str_number.as);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 46)
        {
            long temp = stat_code_str_number.as;
            printf("%c %ld", 186, stat_code_str_number.as);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }
    }
    printf(" %c\n", 186);
    /* ======================================================= */

    /* вывод статистики по .inc файлам и строкам */
    for(int k = 0; k < 63; k++)
    {
        if(k == 0)
        {
            printf("%c .inc", 186);
            k += 6;
        }

        printf(" ");
        if(k == 15)
        {
            long temp = stat_files_ext.inc;
            printf("%c %d", 186, stat_files_ext.inc);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 31)
        {
            long temp = stat_all_str_number.inc;
            printf("%c %ld", 186, stat_all_str_number.inc);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 46)
        {
            long temp = stat_code_str_number.inc;
            printf("%c %ld", 186, stat_code_str_number.inc);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }
    }
    printf(" %c\n", 186);
    /* ==================================================== */

    /* разделение между строками и столбцами со статистикой файлов и итогом по всем файлам и строкам */
    printf("%c", 204);
    for(int k = 0; k < 63; k++)
    {
        if(k == 15) printf("%c", 206);
        if(k == 31) printf("%c", 206);
        if(k == 46) printf("%c", 206);
        printf("%c", 205);
    }
    printf("%c\n", 185);
    /* ============================================================================================== */

    /* вывод итога по всем файлам и строкам */
    for(int k = 0; k < 63; k++)
    {
        if(k == 0)
        {
            printf("%c TOTAL", 186);
            k += 7;
        }

        printf(" ");
        if(k == 15)
        {
            long temp = stat_files_ext.inc + stat_files_ext.c + stat_files_ext.cpp + stat_files_ext.as + stat_files_ext.h;
            printf("%c %d", 186, stat_files_ext.inc + stat_files_ext.c + stat_files_ext.cpp + stat_files_ext.as + stat_files_ext.h);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 31)
        {
            long temp = stat_all_str_number.inc + stat_all_str_number.as + stat_all_str_number.c + stat_all_str_number.cpp + stat_all_str_number.h;
            printf("%c %ld", 186, stat_all_str_number.inc + stat_all_str_number.as + stat_all_str_number.c + stat_all_str_number.cpp + stat_all_str_number.h);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }

        if(k == 46)
        {
            long temp = stat_code_str_number.inc + stat_code_str_number.as + stat_code_str_number.c + stat_code_str_number.cpp + stat_code_str_number.h;
            printf("%c %ld", 186, stat_code_str_number.inc + stat_code_str_number.as + stat_code_str_number.c + stat_code_str_number.cpp + stat_code_str_number.h);
            temp = Length_number(temp);
            if(temp != 0) k += temp + 1;
        }
    }
    printf(" %c\n", 186);
    /* ================================================= */

    // завершающая часть обводки
    printf("%c", 200);
    for(int k = 0; k < 63; k++)
    {
        if(k == 15) printf("%c", 202);
        if(k == 31) printf("%c", 202);
        if(k == 46) printf("%c", 202);
        printf("%c", 205);
    }
    printf("%c\n", 188);
}
