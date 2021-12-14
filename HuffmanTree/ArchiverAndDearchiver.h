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
void fileSize(FILE*);

// function:
// arguments:
void tree(char*, const int);

// function:
// arguments:
void stringTraversalTree(Node*);


// function:
// arguments:
void archiver(FILE*, FILE*, HuffmanCode[], uint16_t);