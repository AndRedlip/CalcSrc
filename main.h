#ifndef _main_h_
#define _main_h_

// максимальная длина для строки которая берется из файлов
#define C_STR_LENGTH_MAX 10000

// максимальная длина имени файлов
#define C_FILES_NAMES_LENGTH_MAX 500

// максимальное кол-во файлов
#define C_FILES_NUMBER_MAX 20000

typedef struct stat_files_ext_t
{
    unsigned int c;
    unsigned int cpp;
    unsigned int as;
    unsigned int inc;
    unsigned int h;
} stat_files_ext_tag;

typedef struct stat_all_str_number_t
{
    unsigned long c;
    unsigned long cpp;
    unsigned long as;
    unsigned long inc;
    unsigned long h;
} stat_all_str_number_tag;

typedef struct stat_code_str_number_t
{
    unsigned long c;
    unsigned long cpp;
    unsigned long as;
    unsigned long inc;
    unsigned long h;
} stat_code_str_number_tag;

extern struct stat_all_str_number_t stat_all_str_number;

extern struct stat_code_str_number_t stat_code_str_number;

extern struct stat_files_ext_t stat_files_ext;

// переменные для статистики
extern int int_dir_total_scan, int_files_total_scan;

#endif // _main
