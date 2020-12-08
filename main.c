/*
 * File:   main.c
 * Author: Andrew Vorobkalo
 *
 * Created on 4 December 2020 y., 15:57
 * Email: andrejvorobkalo146@gmail.com
 * Telegram: @AWR_2709
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

#include "out.h"
#include "main.h"

#define C_CLEAR 1
#define C_DONT_CLEAR 2

#define C_LINE_FEED 10
#define C_SYM_NEWPATH 92  // '\'

typedef enum {false, true} boolean;

typedef enum {e_ext_nf, e_ext_cpp, e_ext_c, e_ext_as, e_ext_h, e_ext_inc} ext ;

struct stat_all_str_number_t stat_all_str_number;

struct stat_code_str_number_t stat_code_str_number;

struct stat_files_ext_t stat_files_ext;

// ���������� ��� ������ ����
time_t curr_t;
time_t futur_t;

// ���������� �� ������� ������������ ����� �������������
int int_index_write;

// ���������� � ������� ���e������ �������� ����� � ����� ����������� ���������
int int_file_curr;

// ���������� ��� ����������
int int_dir_total_scan, int_files_total_scan;

// ������ � ���������� � �����-�� ����������
char uc_directory_and_file_names[C_FILES_NUMBER_MAX][C_FILES_NAMES_LENGTH_MAX];

// ������ � ������� ������������� � �����-�� ����������
char uc_subdirectories_names[C_FILES_NUMBER_MAX][C_FILES_NAMES_LENGTH_MAX];

// ������ � ������� ������������ ������ �� �����
char uc_str[C_STR_LENGTH_MAX];

/* =====================================================================
 *
 * Is_directory
 *
 * =====================================================================
 * � ������� �� ����� ���������
 * ������� ��������� ������ �� �� ��� ��� ����������
 * ���� ��� �� ��� ���������� �� ��������� TRUE
 *
 *  ����:                               ��������� �� ������ � ������
 *
 *  �����:                              0 (false)   �� ����������
 *                                      1 (true)    ����������
 */
boolean Is_directory(char* ptr_path)
{
    DIR *dir;

    if((dir = opendir(ptr_path)) != NULL)  // ���� � ������ ������������� �� ������� ������ true
    {
        return true;

        closedir(dir);
    }
    else return false;
}

/* =====================================================================
 *
 * Search_subdirectories_in_a_directory
 *
 * =====================================================================
 * �� ����� ������ � ���������� �����-�� ����������
 * ������� ��������� ��� ����������
 * ���� ������� ������������� �� ���������� �� �����+���� � ��������� ������ uc_subdirectories_names
 *
 *  ����:
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������ � �����-�� ����������
 *                                      uc_subdirectories_names     ������ � ������� � ����� �������������
 *                                      int_index_write             ���������� �� ������� ������������ ����� ������������� � ������ � ����
 *                                      C_SYM_NEWPATH               ������ ������� �������� ����� ������ ������������� ('\')
 *                                      C_FILES_NUMBER_MAX          ������������ ���-�� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ����� ���� + ����� ������
 *
 *  �����:                              uc_subdirectories_names     ������ � ������� � ����� �������������
 */
void Search_subdirectories_in_a_directory()
{
    signed int j, i = 1;

    char* ptr_path;

    while((++i < C_FILES_NUMBER_MAX) && (uc_directory_and_file_names[i][0] != 0))
    {
        j = -1;

        ptr_path = (char*)(&uc_directory_and_file_names[i]);

        if(Is_directory(ptr_path) == true)
        {
            while((++j < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[i][j] != 0)) uc_subdirectories_names[int_index_write][j] = uc_directory_and_file_names[i][j];

            uc_subdirectories_names[int_index_write][j] = C_SYM_NEWPATH;
            uc_subdirectories_names[int_index_write][j+1] = 0;

            int_index_write++;
        }
    }
}

/* =====================================================================
 *
 * Is_code_as
 *
 * =====================================================================
 * ������� ������� ���������� ��� ���������� � ������ �� ����� � ������������ .as .asm
 * ���� ��� ����������� �� ��������� false
 * ���� ��� ������ ���� � ������������� �������� ��� ��� ���, �� ��������� true
 *
 *  ����:
 *    ���������� ����������:            uc_str           ������ ��� ��������� ������
 *                                      C_STR_LENGTH_MAX ������������ ����� ������
 *
 *  �����:                              0 (false)       ������ � ������������
 *                                      1 (true)        ������ � ����� (� ������������ �������� ��� ���)
 */
boolean Is_code_as()
{
    signed int i = -1;
    int int_symbols = 0;

    if(uc_str[0] == ';') return false;

    while((++i < C_STR_LENGTH_MAX) && (uc_str[i] != 0))
    {
        if((uc_str[i] >= '!') && (uc_str[i] <= '}') && (uc_str[i] != ';') && (uc_str[i] != ' ') && (uc_str[i] != '/')) int_symbols++;

        if((uc_str[i] == '/') && (uc_str[i+1] == '/') && (int_symbols < 1)) return false;

        if((uc_str[i] == ';') && (int_symbols < 1)) return false;
    }

    if(int_symbols < 1) return false;

    return true;
}

/* =====================================================================
 *
 * Is_code_c
 *
 * =====================================================================
 * ������� ������� ���������� ��� ���������� � ������ �� �����
 * ���� ��� ����������� �� ��������� false
 * ���� ��� ������ ���� � ������������� �������� ��� ��� ���, �� ��������� true
 *
 *  ����:                               �����           ����� ��� ������� ���������� �� ��������� ������������
 *    ���������� ����������:            uc_str          ������ ��� ��������� ������
 *                                      C_LINE_FEED     ������� ������
 *                                      C_CLEAR         ����� ��� ������� ��������� ���������� �� ��������� ������������
 *                                      C_DONT_CLEAR    ����� ��� ������� ���������� �� ��������� ������������ �� ���������
 *
 *  �����:                              0 (false)       ������ � ������������
 *                                      1 (true)        ������ � ����� (� ������������ �������� ��� ���)
 */
boolean Is_code_c(int int_option)
{
    signed int i = -1;
    int int_symbols = 0;

    int int_large_comment = 0;        // ���������� �������� � 1 ����� ������� ����������� (/* */) �������������

    static int int_comments = 0;

    if(int_option == C_CLEAR)
    {
        int_comments = 0;
        return false;
    }

    if((uc_str[0] == C_LINE_FEED) && (uc_str[1] == 0)) return false;    // ������ ������

    while((++i < C_STR_LENGTH_MAX) && (uc_str[i] != 0))
    {
        if((uc_str[i] == '/') && (uc_str[i+1] == '*') && (int_symbols < 1)) int_comments = 1;                // ������ � ������� �������� ����������� '/*'

        if((uc_str[i] >= '!') && (uc_str[i] <= '}') && (uc_str[i] != '/') && (uc_str[i] != ' ') && (uc_str[i] != 10) && (int_comments == 0)) int_symbols++;

        if((uc_str[i] == '/') && (uc_str[i+1] == '/') && (int_symbols < 1) && (int_comments == 0)) int_comments = 2;   // ������ � ������������ '//'

        if((int_comments == 1) && (uc_str[i] == '*') && (uc_str[i+1] == '/'))
        {
            int_large_comment = 1;
            int_comments = 0;         // ����� �������� ����������� '*/'
        }
    }

    if((int_large_comment == 1) && (int_symbols < 1)) return false;     // ���� ������� ����������� (/* */) ����������, � ����� ���� ��� ����, �� ��������� false

    if(int_comments == 2)                       // ����������� '//'
    {
        int_comments = 0;
        return false;
    }

    if(int_symbols < 1) return false;           // ������ ������

    if(int_comments == 1) return false;         // �� ��� ��� ������� �����������

    return true;          // ������ � �����
}

/* =====================================================================
 *
 * Is_ext_c
 *
 * =====================================================================
 * ������� ������� ������� ������ �� ������� uc_directory_and_file_names �� ������� ������� �� �����
 * � ����������,�������� �� ���� � ������ �������� ������ � ����������� .c
 *
 *  ����:                               ������ ������ ����������� �� ����
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ������ ����� ���� + ����� �����
 *
 *  �����:                              0 (false)                  � ����� ���������� �� .c
 *                                      1 (true)                   ���� � ����������� .c
 */
boolean Is_ext_c(int i)
{
    signed int j = -1;

    while((++j < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[i][j] != 0))
    {
        if(j + 2 < C_FILES_NAMES_LENGTH_MAX)
        {
            if((uc_directory_and_file_names[i][j] == '.') && (uc_directory_and_file_names[i][j+1] == 'c') && (uc_directory_and_file_names[i][j+2] == 0)) return true;
        }
    }

    return false;
}

/* =====================================================================
 *
 * Is_ext_cpp
 *
 * =====================================================================
 * ������� ������� ������� ������ �� ������� uc_directory_and_file_names �� ������� ������� �� �����
 * � ����������,�������� �� ���� � ������ �������� ������ � ����������� .cpp
 *
 *  ����:                               ������ ������ ����������� �� ����
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ������ ����� ���� + ����� �����
 *
 *  �����:                              0 (false)                   � ����� ���������� �� .cpp
 *                                      1 (true)                    ���� � ����������� .cpp
 */
boolean Is_ext_cpp(int i)
{
    signed int j = -1;

    while((++j < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[i][j] != 0))
    {
        if(j + 4 < C_FILES_NAMES_LENGTH_MAX)
        {
            if((uc_directory_and_file_names[i][j] == '.') && (uc_directory_and_file_names[i][j+1] == 'c') &&
             (uc_directory_and_file_names[i][j+2] == 'p') && (uc_directory_and_file_names[i][j+3] == 'p') && (uc_directory_and_file_names[i][j+4] == 0)) return true;
        }
    }

    return false;
}

/* =====================================================================
 *
 * Is_ext_h
 *
 * =====================================================================
 * ������� ������� ������� ������ �� ������� uc_directory_and_file_names �� ������� ������� �� �����
 * � ����������,�������� �� ���� � ������ �������� ������ � ����������� .h
 *
 *  ����:                               ������ ������ ����������� �� ����
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ������ ����� ���� + ����� �����
 *
 *  �����:                              0 (false)                  � ����� ���������� �� .h
 *                                      1 (true)                   ���� � ����������� .h
 */
boolean Is_ext_h(int i)
{
    signed int j = -1;

    while((++j < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[i][j] != 0))
    {
        if(j + 2 < C_FILES_NAMES_LENGTH_MAX)
        {
            if((uc_directory_and_file_names[i][j] == '.') && (uc_directory_and_file_names[i][j+1] == 'h') && (uc_directory_and_file_names[i][j+2] == 0)) return true;
        }
    }

    return false;
}

/* =====================================================================
 *
 * Is_ext_as
 *
 * =====================================================================
 * ������� ������� ������� ������ �� ������� uc_directory_and_file_names �� ������� ������� �� �����
 * � ����������,�������� �� ���� � ������ �������� ������ � ����������� .as/asm
 *
 *  ����:                               ������ ������ ����������� �� ����
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ������ ����� ���� + ����� �����
 *
 *  �����:                              0 (false)                  � ����� ���������� �� .as/asm
 *                                      1 (true)                   ���� � ����������� .as/asm
 */
boolean Is_ext_as(int i)
{
    signed int j = -1;

    while((++j < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[i][j] != 0))
    {
        if(j + 4 < C_FILES_NAMES_LENGTH_MAX)
        {
            if((uc_directory_and_file_names[i][j] == '.') && (uc_directory_and_file_names[i][j+1] == 'a') &&
             (uc_directory_and_file_names[i][j+2] == 's') && (uc_directory_and_file_names[i][j+3] == 'm') && (uc_directory_and_file_names[i][j+4] == 0)) return true;
        }

        if(j + 4 < C_FILES_NAMES_LENGTH_MAX)
        {
            if((uc_directory_and_file_names[i][j] == '.') && (uc_directory_and_file_names[i][j+1] == 'a') &&
             (uc_directory_and_file_names[i][j+2] == 's') && (uc_directory_and_file_names[i][j+3] == 0)) return true;
        }

    }

    return false;
}

/* =====================================================================
 *
 * Is_ext_inc
 *
 * =====================================================================
 * ������� ������� ������� ������ �� ������� uc_directory_and_file_names �� ������� ������� �� �����
 * � ����������,�������� �� ���� � ������ �������� ������ � ����������� .inc
 *
 *  ����:                               ������ ������ ����������� �� ����
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ������ ����� ���� + ����� �����
 *
 *  �����:                              0 (false)                  � ����� ���������� �� .inc
 *                                      1 (true)                   ���� � ����������� .inc
 */
boolean Is_ext_inc(int i)
{
    signed int j = -1;

    while((++j < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[i][j] != 0))
    {
        if(j + 4 < C_FILES_NAMES_LENGTH_MAX)
        {
            if((uc_directory_and_file_names[i][j] == '.') && (uc_directory_and_file_names[i][j+1] == 'i') &&
             (uc_directory_and_file_names[i][j+2] == 'n') && (uc_directory_and_file_names[i][j+3] == 'c') && (uc_directory_and_file_names[i][j+4] == 0)) return true;
        }
    }

    return false;
}

/* =====================================================================
 *
 * Search_source_files_in_directory
 *
 * =====================================================================
 * ������� �������� ������ ����������, ������� ���������� ���������� ����� ( .c/.cpp/.as/.asm/.h/.inc )
 * ���� ������ ���� �� ��� ������, �� ��������� 0
 * (� 0 ������� ����� ���� �� �����, ��� ��� ��� ��������� ������ �������� Get_contents_folder ������ ��� ������ ������ '.')
 *
 *  ����:                               ������ ������ �� ������� �������� ����� ������� �����
 *    ���������� ����������:            uc_directory_and_file_names ������ � ������� ������
 *                                      C_FILES_NUMBER_MAX          ������������ ���-�� ������
 *                                      int_file_curr               ���������� ������� ������ ������ ����������
 *
 *  �����:                              >0                  ����� ������ � ������ � ����� ������� �����
 *                                      0                   ���� �� ������
 */
int Search_source_files_in_directory(int i)
{
    while((uc_directory_and_file_names[i][0] != 0) && (++i < C_FILES_NUMBER_MAX))
    {
        if(Is_ext_c(i) == true)
        {
            int_file_curr = e_ext_c;
            return i;
        }

        if(Is_ext_cpp(i) == true)
        {
            int_file_curr = e_ext_cpp;
            return i;
        }

        if(Is_ext_h(i) == true)
        {
            int_file_curr = e_ext_h;
            return i;
        }

        if(Is_ext_as(i) == true)
        {
            int_file_curr = e_ext_as;
            return i;
        }

        if(Is_ext_inc(i) == true)
        {
            int_file_curr = e_ext_inc;
            return i;
        }

    }

    int_file_curr = e_ext_nf;

    return 0;
}

/* =====================================================================
 *
 * Get_contents_folder
 *
 * =====================================================================
 * ������� ������� �������� ���������� ���������� ������� ��������� �� ���������
 * ���������� ���������� � ���� � ������ uc_directory_and_file_names
 * �� ������ ���������� ������ � ���������� ���������� + � �����
 *
 *  ����:                               ��������� �� ������ � ����� ����������
 *    ���������� ����������:            uc_directory_and_file_names      ����� ���� ��������e��� ���������� ���������� ������ � �����
 *
 *  �����:                              uc_directory_and_file_names      ������ � ���������� � �����-�� ����������
 */
void Get_contents_folder(char* ptr_path)
{
    char* ptr_path_start;
    ptr_path_start = ptr_path;

    int i = 0;

    DIR *dir;
    struct dirent *ent;

    if((dir = opendir(ptr_path)) != NULL)
    {
        while((ent = readdir(dir)) != NULL)
        {
            int j = 0, m = 0;
            ptr_path = ptr_path_start;

            while(*ptr_path != 0)            // ��������� ���� ����� ������� ����� �����
            {
                uc_directory_and_file_names[i][j] = *ptr_path;
                ptr_path++;
                j++;
            }

            while(ent->d_name[m] != 0)       // ��������� ��� ����� ����� ����
            {
                uc_directory_and_file_names[i][j] = ent->d_name[m];
                j++;
                m++;
            }

            uc_directory_and_file_names[i][j] = 0;

            i++;
        }

        closedir(dir);
    }
}

/* =====================================================================
 *
 * Clear_array_dir_path
 *
 * =====================================================================
 * ������� ������� ������� ������ � �����
 *
 *  ����:                               ��������� �� ������ � �����
 */
void Clear_array_dir_path(char* ptr)
{
    while(*ptr != 0)        // ������� ������� ����
    {
        *ptr = 0;
        ptr++;
    }
}

/* =====================================================================
 *
 * Clear_array_names_files
 *
 * =====================================================================
 * ������� ������� ������� ������ � ������� ������
 *
 *  ����:
 *    ���������� ����������:            C_FILES_NUMBER_MAX          ������������ ���-�� ������
 *                                      C_FILES_NAMES_LENGTH_MAX    ������������ ������ ����� ���� + ����� �����
 *                                      uc_directory_and_file_names ������ � �������
 *
 */
void Clear_array_names_files()
{
    signed int m = -1, k;

    while((++m < C_FILES_NUMBER_MAX) && (uc_directory_and_file_names[m][0] != 0))  // ������� ������ ������ ��� ������ ����� � ������ �������������
    {
        k = -1;

        while((++k < C_FILES_NAMES_LENGTH_MAX) && (uc_directory_and_file_names[m][k] != 0)) uc_directory_and_file_names[m][k] = 0;
    }
}

/* =====================================================================
 *
 * Clear_array_names_sub_dir
 *
 * =====================================================================
 * ������� ������� ������� ������ � ������� �������������
 *
 *  ����:
 *    ���������� ����������:            C_FILES_NUMBER_MAX           ������������ ���-�� ������
 *                                      C_FILES_NAMES_LENGTH_MAX     ������������ ������ ����� ���� + ����� �����
 *                                      uc_subdirectories_names      ������ � ���������������
 *
 */
void Clear_array_names_sub_dir()
{
    signed int m = -1, k;

    while((++m < C_FILES_NUMBER_MAX) && (uc_subdirectories_names[m][0] != 0))  // ������� ������ ������ ��� ������ ����� � ������ �������������
    {
        k = -1;

        while((++k < C_FILES_NAMES_LENGTH_MAX) && (uc_subdirectories_names[m][k] != 0)) uc_subdirectories_names[m][k] = 0;
    }
}

/* =====================================================================
 *
 * Input_path
 *
 * =====================================================================
 * ������� ��� ����� ���� ���������� ������
 * ���� ������ ���� � ����������, ���� �� ����� ��� '\'
 * �� ������� ������������� ������ '\' � 0 � �����
 *
 *  ����:                               ��������� �� ������ ���� �������� ����
 *    ���������� ����������:            C_SYM_NEWPATH   ������ '\'
 *
 *  �����:                              ���� � ���������� ������� ������ ����
 */
void Input_path(char* path)
{
    printf("\nEnter the directory path - ");
    gets(path);

    char uc_last_symb = 0;

    while(*path != 0)
    {
        uc_last_symb = *path;
        path++;
    }

    if((*path == 0) && (uc_last_symb != C_SYM_NEWPATH))
    {
        *path = C_SYM_NEWPATH;

        path++;

        *path = 0;
    }
}

/* =====================================================================
 *
 * Record_new_path
 *
 * =====================================================================
 * ������� ������ ����� ������������� ������ ������
 *
 *  ����:                               ��������� �� ������ ���� ������������ ����� ����, ������ ������ ���������� ����� ����
 *    ���������� ����������:            C_FILES_NAMES_LENGTH_MAX          ������������ ������ ����� ���� + ����� �����
 *                                      uc_subdirectories_names           ������ � ���������������
 *
 *
 */
void Record_new_path(char* ptr_dir, int int_index_new_path)
{
    signed int i = -1;

    while((++i < C_FILES_NAMES_LENGTH_MAX) && (uc_subdirectories_names[int_index_new_path][i] != 0))  // ��������� ���� �������������
    {
        *ptr_dir = uc_subdirectories_names[int_index_new_path][i];
        ptr_dir++;
    }

    *ptr_dir = 0;
}

/* =====================================================================
 *
 * Count_files_in_file_list
 *
 * =====================================================================
 * ������� ������� ������� ����� ������ � ���� �����
 *
 *  ����:
 *    ���������� ����������:            C_FILES_NUMBER_MAX          ������������ ���-�� ������
 *                                      uc_directory_and_file_names ������ � ���������������
 *                                      int_files_total_scan        ���������� �������� ���������������� ������
 *
 */
void Count_files_in_directory()
{
    signed int i = -1;

    char* ptr_path;

    while((++i < C_FILES_NUMBER_MAX) && (uc_directory_and_file_names[i][0] != 0))
    {
        ptr_path = (char*)(&uc_directory_and_file_names[i]);

        if(Is_directory(ptr_path) == false) int_files_total_scan++; // ������� ��� ����� �� ������ ������ (�� ������ �������������)
    }
}

/* =====================================================================
 *
 * Status_bar
 *
 * =====================================================================
 * ������� ������ '.' �� ����� ������������ ����������
 * ������� ����� ��� ���� ����� ���� �� ������� ��� ��������� ������� �� ����� ������������
 *
 *  ����:
 *    ���������� ����������:            futur_t     ���������� � �������� �������� (+2��� �� ��������)
 *                                      curr_t      ������� �����
 */
void Status_bar()
{
    futur_t = time(NULL);

    if(curr_t + 2 <= futur_t)
    {
        curr_t = futur_t;
        printf(".");
    }
}

/* =====================================================================
 *
 * Is_txt_file
 *
 * =====================================================================
 * ������� �������� �����
 * ������� ��������� ���� �� �� ��� �� �������� ��������� � ��������.
 * ��������� true ���� ���� ���������
 *
 *  ����:
 *    ���������� ����������:                C_LINE_FEED             ������ �������� ������
 *                                          C_STR_LENGTH_MAX        ������������ ����� ������
 *
 *  �����:                                  0 (false)   ���� �� ���������
 *                                          1 (true)    ���� ���������
 */
boolean Is_txt_file()
{
    #define C_BYTES_FOR_CHECK 10    // ���-�� ���� ��� ��������
    #define C_SYM_HORIZONTAL_TAB 9  // ������ ���������

    signed int i = -1;

    if(uc_str[0] == 0) return false;

    if((uc_str[0] == C_LINE_FEED) && (uc_str[1] == 0)) return true;

    while((++i < C_BYTES_FOR_CHECK) && (i < C_STR_LENGTH_MAX) && (uc_str[i] != 0))
    {
        if( (uc_str[i] == C_SYM_HORIZONTAL_TAB) || ((uc_str[i] >= ' ') && (uc_str[i] <= '}')) || ((uc_str[i] >= -128) && (uc_str[i] <= -1)) || (uc_str[i] == C_LINE_FEED) || (uc_str[i] == 0)) continue;

        return false;
    }

    return true;
}

/* =====================================================================
 *
 * Search_and_read_all_source_files_in_directory
 *
 * =====================================================================
 * ������� ������� ���� ��� �������� ����� � �����-�� ���������� � ������ ��
 * ���� ������� ���� ����� � ����� ����
 *
 *  ����:
 *    ���������� ����������:                int_file_curr               ���������� ������� ������ ������ ����������
 *                                          stat_files_ext_t            ��������� ������� ������ � ���� ���������� ��� �������� �������� ������ � ������� ������������
 *                                          stat_all_str_number_t       ��������� ������� ������ � ���� ���������� ��� �������� ����� � �������� ������ � ������� ������������
 *                                          stat_code_str_number_t      ��������� ������� ������ � ���� ���������� ��� �������� ����� ���� � �������� ������ � ������� ������������
 *                                          C_CLEAR                     ����� ������� ���������� � ������� Is_code_c ��� ������� ����������� ����������
 *
 *
 *  �����:                                  0 (false)   ���� �� ���������
 *                                          1 (true)    ���� ���������
 */
void Search_and_read_all_source_files_in_directory()
{
    int i = 0, bytes;

    FILE *in;

    char* ptr_str_from_file;
    ptr_str_from_file = (char*)(&uc_str[0]);

    int int_source_file_str_all_number = 0;
    int int_source_file_str_code_number = 0;

    while(1)
    {
        i = Search_source_files_in_directory(i);            // ���� ������ �����

        if(i == 0) break;                       // ���� ������ ������ ���

        in = fopen(uc_directory_and_file_names[i], "r");     // ��������� ��������� ����
        if(in == NULL) printf("\nERROR TO OPEN FILE!\n");

        fseek(in, 0 , SEEK_END);                 // ��������� ������ ������ ������

        bytes = ftell(in);                       // �������� ���-�� �������� � ��������� bytes

        fseek(in, 0, SEEK_SET);                  // ��������� �� ������ ����� (����� ������ ��� � ������ ������)

        int_source_file_str_code_number = 0;
        int_source_file_str_all_number = 0;

        while(fgets(ptr_str_from_file, bytes, in) != NULL)          // ���� ���� �� �������� ���� �� �����
        {
            if(Is_txt_file() == false) break;

            if((int_file_curr == e_ext_as) || (int_file_curr == e_ext_inc))
            {
                if(Is_code_as() == true) int_source_file_str_code_number++;  // ������� ������� ����� ���� (��� ������������)
            }

            if((int_file_curr == e_ext_c) || (int_file_curr == e_ext_cpp) || (int_file_curr == e_ext_h))
            {
                if(Is_code_c(C_DONT_CLEAR) == true) int_source_file_str_code_number++;  // ������� ������� ����� ���� (��� ������������)
            }

            int_source_file_str_all_number++;  // ������� ������� ����� ����� � ����������
        }

        Is_code_c(C_CLEAR);     // ������� ���������� �� �������� ������������

        fclose(in);

        if(int_file_curr == e_ext_c)
        {
            stat_files_ext.c++;
            stat_all_str_number.c  += int_source_file_str_all_number;
            stat_code_str_number.c += int_source_file_str_code_number;
        }

        if(int_file_curr == e_ext_cpp)
        {
            stat_files_ext.cpp++;
            stat_all_str_number.cpp  += int_source_file_str_all_number;
            stat_code_str_number.cpp += int_source_file_str_code_number;
        }

        if(int_file_curr == e_ext_h)
        {
            stat_files_ext.h++;
            stat_all_str_number.h  += int_source_file_str_all_number;
            stat_code_str_number.h += int_source_file_str_code_number;
        }

        if(int_file_curr == e_ext_as)
        {
            stat_files_ext.as++;
            stat_all_str_number.as  += int_source_file_str_all_number;
            stat_code_str_number.as += int_source_file_str_code_number;
        }

        if(int_file_curr == e_ext_inc)
        {
            stat_files_ext.inc++;
            stat_all_str_number.inc  += int_source_file_str_all_number;
            stat_code_str_number.inc += int_source_file_str_code_number;
        }
    }
}

/* =====================================================================
 *
 * Setup
 *
 * =====================================================================
 * ������� ������� ������� ���������, ����������, �������
 *
 *  ����:                                   ��������� �� ������ � ����� ��� ��� �������
 *    ���������� ����������:                int_file_curr               ���������� ������� ������ ������ ����������
 *                                          stat_files_ext_t            ��������� ������� ������ � ���� ���������� ��� �������� �������� ������ � ������� ������������
 *                                          stat_all_str_number_t       ��������� ������� ������ � ���� ���������� ��� �������� ����� � �������� ������ � ������� ������������
 *                                          stat_code_str_number_t      ��������� ������� ������ � ���� ���������� ��� �������� ����� ���� � �������� ������ � ������� ������������
 *                                          int_index_write             ���������� �� ������� ���� ������ �������������
 *                                          curr_t                      ���������� ������� �������� ������� �����, ��������� ������ ��� ������ ����� . � ������ ����
 *                                          futur_t                     ���������� ������� �������� ��������� ������������� ������� �����
 *                                          int_files_total_scan        ���������� ������� �������� ����� ���������������� ������
 *                                          int_dir_total_scan          ���������� ������� �������� ����� ���������������� ����������
 *
 */
void Setup(char* ptr_dir_path)
{
    Clear_array_dir_path(ptr_dir_path);
    Clear_array_names_files();
    Clear_array_names_sub_dir();

    curr_t = time(NULL);
    futur_t = time(NULL);

    stat_all_str_number.as = 0;
    stat_all_str_number.c = 0;
    stat_all_str_number.cpp = 0;
    stat_all_str_number.h = 0;
    stat_all_str_number.inc = 0;

    stat_code_str_number.as = 0;
    stat_code_str_number.c = 0;
    stat_code_str_number.cpp = 0;
    stat_code_str_number.h = 0;
    stat_code_str_number.inc = 0;

    stat_files_ext.as = 0;
    stat_files_ext.c = 0;
    stat_files_ext.cpp = 0;
    stat_files_ext.h = 0;
    stat_files_ext.inc = 0;

    int_index_write = 0;

    int_file_curr = 0;

    int_files_total_scan = 0;
    int_dir_total_scan = 0;
}

int main()
{
    // ������ ��� ����� ���������� ���� � ����������
    char uc_dir_path[C_FILES_NAMES_LENGTH_MAX];

    int int_index_search = 0;

    char* ptr_dir_path;

    //ptr_str_from_file = (char*)(&uc_str[0]);
    ptr_dir_path = (char*)(&uc_dir_path[0]);

    Setup(ptr_dir_path);  // ������� ���� �������� � ���������� ����� ������������� ����� ���������� */

    printf("\nEXAMPLE: C:\\source\n");

    while(uc_dir_path[0] != '2')                // ���� ���� ���� �� ������� ��� (2)
    {
        Setup(ptr_dir_path);  // ������� ���� �������� � ���������� ����� ������������� ����� ����������
        int_index_search = 0;

        Input_path(ptr_dir_path);        // ���� ���������� ������� ���������

        if(Is_directory(ptr_dir_path) == false)    // �������� ��� ����� ���������� ����������
        {
            printf("\nINVALID PATH!\nEXAMPLE: C:\\source\n");
            continue;
        }

        while(1) // ���� ���� ��� ������������� �� ����� ��������������
        {
            Get_contents_folder(ptr_dir_path);    // �������� ������ ������ � �����-�� ����������/�������������

            Count_files_in_directory();  // ������� ������� ������ � ������ ������ (�� ������ �������������)

            Search_and_read_all_source_files_in_directory();    // ���� � ������ ��� �������� ����� � ������� ����������

            Status_bar();

            Search_subdirectories_in_a_directory();       // �������� ������������� � ������ ������

            if(uc_subdirectories_names[int_index_search][0] == 0) break;    // ���� ��� ������������� ��������� �� �����������

            Clear_array_dir_path(ptr_dir_path);                  // ������� ������� ����

            Record_new_path(ptr_dir_path, int_index_search);     // ������ ������ ����

            int_index_search++;
            int_dir_total_scan++;        // ������� ����������� �������������

            Clear_array_names_files();      // ������� ������� ������ ������
        }

        Out_statistic();    // ����� ����������

        printf("Scan again? 1 - YES. 2 - NO.\n\n");
        gets(uc_dir_path);
    }

    printf("\nThe program completed successfully.\n");

    /* ��� ����� ����� ��������� �� ����������� ���� ����� ��������� ������������ */
    int test;

    scanf("%d", &test);
}
