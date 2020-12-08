#include <stdio.h>
#include <stdlib.h>

#include "main.h"

/* =====================================================================
 *
 * Length_number
 *
 * =====================================================================
 * ������� ������� ������� ����� �����
 *
 *  ����:                               �����
 *
 *  �����:                              1   ����� ����� ������� ������ 10
 *                                      >1  ����� ����� ������� ������ ��� ����� 10
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
 * ������� ������� ������� ��� ���������� � �������
 *
 *  ����:
 *    ���������� ����������:            stat_files_ext_t        ��������� ������� ������ � ���� ���������� c ����������� �������� ������ � ������� ������������
 *                                      stat_all_str_number_t   ��������� ������� ������ � ���� ���������� c ����������� ����� � �������� ������ � ������� ������������
 *                                      stat_code_str_number_t  ��������� ������� ������ � ���� ���������� c ����������� ����� ���� � �������� ������ � ������� ������������
 *                                      int_files_total_scan    ���������� �������� ����� ���������������� ������
 *                                      int_dir_total_scan      ���������� �������� ����� ���������������� ����������
 *
 *
 *  �����:                              ����� ���� ���������� � �������
 */
void Out_statistic()
{
    printf("\n\nScanned %d files, %d directories: ", int_files_total_scan, int_dir_total_scan);
    /* ������ ������ � ����������� ������ */
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

    /* ���������� ����� ��������� � �������� */
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

    /* ����� ���������� �� .c ������ � ������� */
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


    /* ����� ���������� �� .cpp ������ � ������� */
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

    /* ����� ���������� �� .h ������ � ������� */
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

    /* ����� ���������� �� .as/asm ������ � ������� */
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

    /* ����� ���������� �� .inc ������ � ������� */
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

    /* ���������� ����� �������� � ��������� �� ����������� ������ � ������ �� ���� ������ � ������� */
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

    /* ����� ����� �� ���� ������ � ������� */
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

    // ����������� ����� �������
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
