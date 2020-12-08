# CalcSrc
The utility searches for source files (type * .c, * .h, * .cpp, * .inc, * .as, * .asm) in the specified folder and all its subfolders. Counts the number of lines in the files found and displays the results.

Description

--------------------------------------------------------------

��������� ������� ��� ���������� �� �������� ������� �������� �� ����� ������ ���: C, C++, Assembler.

�������� ���� ���������� � ������ Games ������������� �� ����� C � �������� ����������. 
������������ ������ C:\Games ��� C:\Games\ . ����� ���� ��������� ���� � ��������� ��� �������� ����� � ������������ .c, .cpp, .h, .as, .asm, .inc � ����� ���������� � �� ��������������. 

����� ������� ������� ����� ������������� �������� ������, ����� ������ � �����. ��� �� ������� ������� ����� ����� � ����������, � ������� ����� ����.

� ������ � ������������ .c, .cpp, .h ��������� ����������� �� ����:
/* */ , // , /** **/ .
� ������ � ������������ .as, .asm, .inc ��������� ����������� �� ����: 
; � // .

--------------------------------------------------------------

The program is designed for statistics on projects that are written in languages such as C, C++, Assembler.

For example, there is a directory named Games located on the C drive in the root directory.
The user enters C:\Games or C:\Games\. After that the program searches and scans all source files with the extensions .c, .cpp, .h, .as, .asm, .inc in the directory itself and its subdirectories.

Counts how many source files, total files and folders are scanned. It also counts how many lines are in the source, and how many lines of code.

In files with the extensions .c, .cpp, .h accepts comments of the type:
/* */, //, /** **/.
In files with the extensions .as, .asm, .inc accepts comments of the type:
; and //.