#include "HuffmanTree.h"

// function: �������� ����� � ����������� ��� ������������
// arguments: char mode - ������, ������������ ������ �������� �����:
// 'r' - ������
// 'w' - ������
// return: FILE* file - ��������� �� �������� ����
FILE* openFile(char);

// function: ����� ������� �����
// arguments: FILE* file - ��������� �� ����
// return: uint32_t file_size - ������ ����� � ��
uint32_t fileSize(FILE*);

// function: ��������� �����
// arguments: FILE* source - ��������� �� �������� ����
FILE* archiver(FILE*);

// function: ������������ �����
// arguments: FILE* archiver - ��������� �� ������ ����
void dearchiver(FILE*);