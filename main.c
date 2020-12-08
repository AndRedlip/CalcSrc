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

// переменные для статус бара
time_t curr_t;
time_t futur_t;

// переменная по которой записываются новые поддиректории
int int_index_write;

// переменная в которую перeдается значение файла с каким расширением сканируем
int int_file_curr;

// переменные для статистики
int int_dir_total_scan, int_files_total_scan;

// массив с содержимым в какой-то директории
char uc_directory_and_file_names[C_FILES_NUMBER_MAX][C_FILES_NAMES_LENGTH_MAX];

// массив с именами поддиректорий в какой-то директории
char uc_subdirectories_names[C_FILES_NUMBER_MAX][C_FILES_NAMES_LENGTH_MAX];

// массив в который записывается строка из файла
char uc_str[C_STR_LENGTH_MAX];

/* =====================================================================
 *
 * Is_directory
 *
 * =====================================================================
 * у функции на входе указатель
 * функция проверяет строку на то что это директория
 * если все же это директория то возращает TRUE
 *
 *  Вход:                               указатель на строку с именем
 *
 *  Выход:                              0 (false)   не директория
 *                                      1 (true)    директории
 */
boolean Is_directory(char* ptr_path)
{
    DIR *dir;

    if((dir = opendir(ptr_path)) != NULL)  // если в строке поддиректория то функция вернет true
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
 * на входе массив с содержимым какой-то директории
 * функция проверяет все содержимое
 * если находит поддиректории то записывает их имена+путь в отдельный массив uc_subdirectories_names
 *
 *  Вход:
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов в какой-то директории
 *                                      uc_subdirectories_names     массив с именами и путем поддиректорий
 *                                      int_index_write             переменная по которой записываются новые поддиректории в массив с ними
 *                                      C_SYM_NEWPATH               символ который ставится после записи поддиректории ('\')
 *                                      C_FILES_NUMBER_MAX          максимальное кол-во файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальная длина пути + имени файлов
 *
 *  Выход:                              uc_subdirectories_names     массив с именами и путем поддиректорий
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
 * функция которая определяет что содержится в строке из файла с распишериями .as .asm
 * если это комментарии то возращает false
 * если это строка кода с комментариями напротив нее или без, то возращает true
 *
 *  Вход:
 *    глобальные переменные:            uc_str           массив где находится строка
 *                                      C_STR_LENGTH_MAX максимальная длина строки
 *
 *  Выход:                              0 (false)       строка с комментарием
 *                                      1 (true)        строка с кодом (с комментарием напротив или без)
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
 * функция которая определяет что содержится в строке из файла
 * если это комментарии то возращает false
 * если это строка кода с комментариями напротив нее или без, то возращает true
 *
 *  Вход:                               опция           опция для очистки переменной со значением комментариев
 *    глобальные переменные:            uc_str          массив где находится строка
 *                                      C_LINE_FEED     перевод строки
 *                                      C_CLEAR         опция при которой очищается переменная со значением комментариев
 *                                      C_DONT_CLEAR    опция при которой переменная со значением комментариев не очищается
 *
 *  Выход:                              0 (false)       строка с комментарием
 *                                      1 (true)        строка с кодом (с комментарием напротив или без)
 */
boolean Is_code_c(int int_option)
{
    signed int i = -1;
    int int_symbols = 0;

    int int_large_comment = 0;        // переменная ставится в 1 когда большой комментарий (/* */) заканчивается

    static int int_comments = 0;

    if(int_option == C_CLEAR)
    {
        int_comments = 0;
        return false;
    }

    if((uc_str[0] == C_LINE_FEED) && (uc_str[1] == 0)) return false;    // пустая строка

    while((++i < C_STR_LENGTH_MAX) && (uc_str[i] != 0))
    {
        if((uc_str[i] == '/') && (uc_str[i+1] == '*') && (int_symbols < 1)) int_comments = 1;                // строка с началом большого комментария '/*'

        if((uc_str[i] >= '!') && (uc_str[i] <= '}') && (uc_str[i] != '/') && (uc_str[i] != ' ') && (uc_str[i] != 10) && (int_comments == 0)) int_symbols++;

        if((uc_str[i] == '/') && (uc_str[i+1] == '/') && (int_symbols < 1) && (int_comments == 0)) int_comments = 2;   // строка с комментарием '//'

        if((int_comments == 1) && (uc_str[i] == '*') && (uc_str[i+1] == '/'))
        {
            int_large_comment = 1;
            int_comments = 0;         // конец большого комментария '*/'
        }
    }

    if((int_large_comment == 1) && (int_symbols < 1)) return false;     // если большой комментарий (/* */) закончился, и после него нет кода, то возращает false

    if(int_comments == 2)                       // комментарий '//'
    {
        int_comments = 0;
        return false;
    }

    if(int_symbols < 1) return false;           // пустая строка

    if(int_comments == 1) return false;         // до сих пор большой комментарий

    return true;          // строка с кодом
}

/* =====================================================================
 *
 * Is_ext_c
 *
 * =====================================================================
 * функция которая смотрит строку из массива uc_directory_and_file_names по индексу который на входе
 * и определяет,является ли файл в строке исходным файлом с расширением .c
 *
 *  Вход:                               индекс строки указывающий на файл
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальный размер длины пути + имени файла
 *
 *  Выход:                              0 (false)                  у файла расширение не .c
 *                                      1 (true)                   файл с расширением .c
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
 * функция которая смотрит строку из массива uc_directory_and_file_names по индексу который на входе
 * и определяет,является ли файл в строке исходным файлом с расширением .cpp
 *
 *  Вход:                               индекс строки указывающий на файл
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальный размер длины пути + имени файла
 *
 *  Выход:                              0 (false)                   у файла расширение не .cpp
 *                                      1 (true)                    файл с расширением .cpp
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
 * функция которая смотрит строку из массива uc_directory_and_file_names по индексу который на входе
 * и определяет,является ли файл в строке исходным файлом с расширением .h
 *
 *  Вход:                               индекс строки указывающий на файл
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальный размер длины пути + имени файла
 *
 *  Выход:                              0 (false)                  у файла расширение не .h
 *                                      1 (true)                   файл с расширением .h
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
 * функция которая смотрит строку из массива uc_directory_and_file_names по индексу который на входе
 * и определяет,является ли файл в строке исходным файлом с расширением .as/asm
 *
 *  Вход:                               индекс строки указывающий на файл
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальный размер длины пути + имени файла
 *
 *  Выход:                              0 (false)                  у файла расширение не .as/asm
 *                                      1 (true)                   файл с расширением .as/asm
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
 * функция которая смотрит строку из массива uc_directory_and_file_names по индексу который на входе
 * и определяет,является ли файл в строке исходным файлом с расширением .inc
 *
 *  Вход:                               индекс строки указывающий на файл
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальный размер длины пути + имени файла
 *
 *  Выход:                              0 (false)                  у файла расширение не .inc
 *                                      1 (true)                   файл с расширением .inc
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
 * функция вызывает разные подфункции, которые определяют расширение файла ( .c/.cpp/.as/.asm/.h/.inc )
 * если нужный файл не был найден, то возращает 0
 * (в 0 индексе файла быть не может, так как при получении файлов функцией Get_contents_folder первые две ячейки заняты '.')
 *
 *  Вход:                               индекс строки от которой начинать поиск нужного файла
 *    глобальные переменные:            uc_directory_and_file_names массив с именами файлов
 *                                      C_FILES_NUMBER_MAX          максимальное кол-во файлов
 *                                      int_file_curr               переменная которая хранит статус расширения
 *
 *  Выход:                              >0                  номер строки с именем и путем нужного файла
 *                                      0                   файл не найден
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
 * функция которая получает содержимое директории которая находится по указателю
 * записывает содержимое и путь в массив uc_directory_and_file_names
 * на выходе получается массив с содержимым директории + с путем
 *
 *  Вход:                               указатель на массив с путем директории
 *    глобальные переменные:            uc_directory_and_file_names      масив куда записываeтся содержимое директории вместе с путем
 *
 *  Выход:                              uc_directory_and_file_names      массив с содержимым в какой-то директории
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

            while(*ptr_path != 0)            // вставляем путь перед началом имени файла
            {
                uc_directory_and_file_names[i][j] = *ptr_path;
                ptr_path++;
                j++;
            }

            while(ent->d_name[m] != 0)       // вставляем имя файла после пути
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
 * функция которая очищает массив с путем
 *
 *  Вход:                               указатель на массив с путем
 */
void Clear_array_dir_path(char* ptr)
{
    while(*ptr != 0)        // очистка старого пути
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
 * функция которая очищает массив с именами файлов
 *
 *  Вход:
 *    глобальные переменные:            C_FILES_NUMBER_MAX          максимальное кол-во файлов
 *                                      C_FILES_NAMES_LENGTH_MAX    максимальный размер длины пути + имени файла
 *                                      uc_directory_and_file_names массив с файлами
 *
 */
void Clear_array_names_files()
{
    signed int m = -1, k;

    while((++m < C_FILES_NUMBER_MAX) && (uc_directory_and_file_names[m][0] != 0))  // очистка списка файлов для записи новых в другой поддиректории
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
 * функция которая очищает массив с именами поддиректорий
 *
 *  Вход:
 *    глобальные переменные:            C_FILES_NUMBER_MAX           максимальное кол-во файлов
 *                                      C_FILES_NAMES_LENGTH_MAX     максимальный размер длины пути + имени файла
 *                                      uc_subdirectories_names      массив с поддиректориями
 *
 */
void Clear_array_names_sub_dir()
{
    signed int m = -1, k;

    while((++m < C_FILES_NUMBER_MAX) && (uc_subdirectories_names[m][0] != 0))  // очистка списка файлов для записи новых в другой поддиректории
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
 * функция для ввода пути директории юзером
 * юзер вводит путь к директории, если на конце нет '\'
 * то функция автоматически ставит '\' и 0 в конец
 *
 *  Вход:                               указатель на массив куда вводится путь
 *    глобальные переменные:            C_SYM_NEWPATH   символ '\'
 *
 *  Выход:                              путь к директории которую указал юзер
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
 * функция записи новой поддиректории вместо старой
 *
 *  Вход:                               указатель на массив куда записывается новый путь, индекс откуда копируется новый путь
 *    глобальные переменные:            C_FILES_NAMES_LENGTH_MAX          максимальный размер длины пути + имени файла
 *                                      uc_subdirectories_names           массив с поддиректориями
 *
 *
 */
void Record_new_path(char* ptr_dir, int int_index_new_path)
{
    signed int i = -1;

    while((++i < C_FILES_NAMES_LENGTH_MAX) && (uc_subdirectories_names[int_index_new_path][i] != 0))  // вписываем путь поддиректории
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
 * функция которая считает число файлов в файл листе
 *
 *  Вход:
 *    глобальные переменные:            C_FILES_NUMBER_MAX          максимальное кол-во файлов
 *                                      uc_directory_and_file_names массив с поддиректориями
 *                                      int_files_total_scan        переменная подсчета просканированных файлов
 *
 */
void Count_files_in_directory()
{
    signed int i = -1;

    char* ptr_path;

    while((++i < C_FILES_NUMBER_MAX) && (uc_directory_and_file_names[i][0] != 0))
    {
        ptr_path = (char*)(&uc_directory_and_file_names[i]);

        if(Is_directory(ptr_path) == false) int_files_total_scan++; // считаем все файлы из списка файлов (не считая поддиректории)
    }
}

/* =====================================================================
 *
 * Status_bar
 *
 * =====================================================================
 * функция вывода '.' во время сканирования директории
 * функция нужна для того чтобы юзер не подумал что программа зависла во время сканирования
 *
 *  Вход:
 *    глобальные переменные:            futur_t     переменная с будуюшим временем (+2сек от текущего)
 *                                      curr_t      текущее время
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
 * функция проверки файла
 * функция проверяет файл на то что он является текстовым и читаемым.
 * возращает true если файл текстовый
 *
 *  Вход:
 *    глобальные переменные:                C_LINE_FEED             символ переноса строки
 *                                          C_STR_LENGTH_MAX        максимальная длина строки
 *
 *  Выход:                                  0 (false)   файл не текстовый
 *                                          1 (true)    файл текстовый
 */
boolean Is_txt_file()
{
    #define C_BYTES_FOR_CHECK 10    // кол-во байт для проверки
    #define C_SYM_HORIZONTAL_TAB 9  // символ табуляции

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
 * функция которая ищет все исходные файлы в какой-то директории и читает их
 * ведя подсчет всех строк и строк кода
 *
 *  Вход:
 *    глобальные переменные:                int_file_curr               переменная которая хранит статус расширения
 *                                          stat_files_ext_t            структура которая хранит в себе переменные для подсчета исходных файлов с разными расширениями
 *                                          stat_all_str_number_t       структура которая хранит в себе переменные для подсчета строк в исходных файлах с разными расширениями
 *                                          stat_code_str_number_t      структура которая хранит в себе переменные для подсчета строк кода в исходных файлах с разными расширениями
 *                                          C_CLEAR                     опция которая передается в функцию Is_code_c для очистки статической переменной
 *
 *
 *  Выход:                                  0 (false)   файл не текстовый
 *                                          1 (true)    файл текстовый
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
        i = Search_source_files_in_directory(i);            // ищем нужные файлы

        if(i == 0) break;                       // если нужных файлов нет

        in = fopen(uc_directory_and_file_names[i], "r");     // открываем найденный файл
        if(in == NULL) printf("\nERROR TO OPEN FILE!\n");

        fseek(in, 0 , SEEK_END);                 // замерение общего объема текста

        bytes = ftell(in);                       // передача кол-ва символов в перемнную bytes

        fseek(in, 0, SEEK_SET);                  // возращаем на начало файла (чтобы читать все с самого начала)

        int_source_file_str_code_number = 0;
        int_source_file_str_all_number = 0;

        while(fgets(ptr_str_from_file, bytes, in) != NULL)          // цикл пока не дочитаем файл до конца
        {
            if(Is_txt_file() == false) break;

            if((int_file_curr == e_ext_as) || (int_file_curr == e_ext_inc))
            {
                if(Is_code_as() == true) int_source_file_str_code_number++;  // считаем сколько строк кода (без комментариев)
            }

            if((int_file_curr == e_ext_c) || (int_file_curr == e_ext_cpp) || (int_file_curr == e_ext_h))
            {
                if(Is_code_c(C_DONT_CLEAR) == true) int_source_file_str_code_number++;  // считаем сколько строк кода (без комментариев)
            }

            int_source_file_str_all_number++;  // считаем сколько строк всего в исходниках
        }

        Is_code_c(C_CLEAR);     // очистка переменной со статусом комментариев

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
 * функция которая очищает структуры, переменные, массивы
 *
 *  Вход:                                   указатель на массив с путем для его очистки
 *    глобальные переменные:                int_file_curr               переменная которая хранит статус расширения
 *                                          stat_files_ext_t            структура которая хранит в себе переменные для подсчета исходных файлов с разными расширениями
 *                                          stat_all_str_number_t       структура которая хранит в себе переменные для подсчета строк в исходных файлах с разными расширениями
 *                                          stat_code_str_number_t      структура которая хранит в себе переменные для подсчета строк кода в исходных файлах с разными расширениями
 *                                          int_index_write             переменная по которой идет запись поддиректорий
 *                                          curr_t                      переменная которая содержит текущее время, обновлясь только при выводе новой . в статус баре
 *                                          futur_t                     переменная которая содержит постоянно обновляющееся текущее время
 *                                          int_files_total_scan        переменная которая содержит число просканированных файлов
 *                                          int_dir_total_scan          переменная которая содержит число просканированных директорий
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
    // массив где будет находиться путь к директории
    char uc_dir_path[C_FILES_NAMES_LENGTH_MAX];

    int int_index_search = 0;

    char* ptr_dir_path;

    //ptr_str_from_file = (char*)(&uc_str[0]);
    ptr_dir_path = (char*)(&uc_dir_path[0]);

    Setup(ptr_dir_path);  // очистка всех массивов и переменных перед сканированием новой директории */

    printf("\nEXAMPLE: C:\\source\n");

    while(uc_dir_path[0] != '2')                // цикл пока юзер не выберет нет (2)
    {
        Setup(ptr_dir_path);  // очистка всех массивов и переменных перед сканированием новой директории
        int_index_search = 0;

        Input_path(ptr_dir_path);        // путь директории которую сканируем

        if(Is_directory(ptr_dir_path) == false)    // проверка что такая директория существует
        {
            printf("\nINVALID PATH!\nEXAMPLE: C:\\source\n");
            continue;
        }

        while(1) // цикл пока все поддиректории не будут просканированы
        {
            Get_contents_folder(ptr_dir_path);    // получаем список файлов в какой-то директории/поддиректории

            Count_files_in_directory();  // считаем сколько файлов в списке файлов (не считая поддиректории)

            Search_and_read_all_source_files_in_directory();    // ищем и читаем все исходные файлы в текущей директории

            Status_bar();

            Search_subdirectories_in_a_directory();       // проверка поддиректорий в списке файлов

            if(uc_subdirectories_names[int_index_search][0] == 0) break;    // если все поддиректории проверены то заканчиваем

            Clear_array_dir_path(ptr_dir_path);                  // очистка старого пути

            Record_new_path(ptr_dir_path, int_index_search);     // запись нового пути

            int_index_search++;
            int_dir_total_scan++;        // подсчет проверенных поддиректорий

            Clear_array_names_files();      // очистка старого списка файлов
        }

        Out_statistic();    // вывод статистики

        printf("Scan again? 1 - YES. 2 - NO.\n\n");
        gets(uc_dir_path);
    }

    printf("\nThe program completed successfully.\n");

    /* это нужно чтобы программа не закрывалась сама после окончания сканирования */
    int test;

    scanf("%d", &test);
}
