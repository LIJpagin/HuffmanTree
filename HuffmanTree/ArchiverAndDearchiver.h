#include "CommonHeader.h"
#include <sys/types.h>
#include "dirent.h"

// function: открыти€ файла с подсказками дл€ пользовател€
// arguments: char mode - символ, определ€ющий способ открыти€ файла:
// 'r' - чтение
// 'w' - запись
// return: FILE* file - указатель на открытый файл
FILE* openFile(char);

// function: вывод размера файла
// arguments: FILE* file - указатель на файл
uint32_t fileSize(FILE*);

// function:
// arguments:
void tree(char*, const int);


// function:
// arguments:
FILE* archiver(FILE*);

void dearchiver(FILE*);