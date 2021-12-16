#include "HuffmanTree.h"

// function: �������� ����� ��� ������
// arguments: char mode - ������, ������������ ���������� �����:
// 'a' - �����
// 'f' - ���� ���������
// return: FILE* file - ��������� �� �������� ����
FILE* openFileForRecording(char);

// function: ����� ������� �����
// arguments: FILE* file - ��������� �� ����
// return: uint32_t file_size - ������ ����� � ��
uint32_t fileSize(FILE*);

// function: ��������� �����
// arguments: FILE* source - ��������� �� �������� ����
// return: FILE* archive - ��������� �� ����� 
FILE* encoder(FILE*);

// function: ���������� �����
// arguments: FILE* encoder - ��������� �� ������ ����
// return: FILE* file - ��������� �� ��������� ���� 
FILE* decoder(FILE*);