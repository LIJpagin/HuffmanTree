#include "CommonHeader.h"
#include <sys/types.h>
#include "dirent.h"

// function: �������� ����� � ����������� ��� ������������
// arguments: char mode - ������, ������������ ������ �������� �����:
// 'r' - ������
// 'w' - ������
// return: FILE* file - ��������� �� �������� ����
FILE* openFile(char);

// function: ����� ������� �����
// arguments: FILE* file - ��������� �� ����
uint32_t fileSize(FILE*);

void stringTraversalTree(FILE*, Node*, char*);

// function:
// arguments:
void tree(char*, const int);


// function: ��������� �����
// arguments: FILE* source - ��������� �� �������� ����
FILE* archiver(FILE*);

// function: ������������ �����
// arguments: FILE* archiver - ��������� �� ������ ����
void dearchiver(FILE*);