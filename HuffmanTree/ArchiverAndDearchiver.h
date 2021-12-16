#include "HuffmanTree.h"

// function: открыти€ файла с подсказками дл€ пользовател€
// arguments: char mode - символ, определ€ющий способ открыти€ файла:
// 'r' - чтение
// 'w' - запись
// return: FILE* file - указатель на открытый файл
FILE* openFile(char);

// function: вывод размера файла
// arguments: FILE* file - указатель на файл
// return: uint32_t file_size - размер файла в  б
uint32_t fileSize(FILE*);

// function: архиваци€ файла
// arguments: FILE* source - указатель на исходный файл
FILE* archiver(FILE*);

// function: разорхиваци€ файла
// arguments: FILE* archiver - указатель на сжатый файл
void dearchiver(FILE*);