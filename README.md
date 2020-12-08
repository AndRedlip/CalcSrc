# CalcSrc
The utility searches for source files (type * .c, * .h, * .cpp, * .inc, * .as, * .asm) in the specified folder and all its subfolders. Counts the number of lines in the files found and displays the results.

Description

--------------------------------------------------------------

Программа создана для статистики по проектам которые написаны на таких языках как: C, C++, Assembler.

Например есть директория с именем Games расположенная на диске C в корневой директории. 
Пользователь вводит C:\Games или C:\Games\ . После чего программа ищет и сканирует все исходные файлы с расширениями .c, .cpp, .h, .as, .asm, .inc в самой директории и ее поддиректориях. 

Ведет подсчет сколько всего проскариванно исходных файлов, всего файлов и папок. Так же считает сколько всего строк в исходниках, и сколько строк кода.

В файлах с расширениями .c, .cpp, .h принимает комментарии по типу:
/* */ , // , /** **/ .
В файлах с расширениями .as, .asm, .inc принимает комментарии по типу: 
; и // .

--------------------------------------------------------------

The program is designed for statistics on projects that are written in languages such as C, C++, Assembler.

For example, there is a directory named Games located on the C drive in the root directory.
The user enters C:\Games or C:\Games\. After that the program searches and scans all source files with the extensions .c, .cpp, .h, .as, .asm, .inc in the directory itself and its subdirectories.

Counts how many source files, total files and folders are scanned. It also counts how many lines are in the source, and how many lines of code.

In files with the extensions .c, .cpp, .h accepts comments of the type:
/* */, //, /** **/.
In files with the extensions .as, .asm, .inc accepts comments of the type:
; and //.