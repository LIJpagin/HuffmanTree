#include "HuffmanTree.h"

// function: открыти€ файла дл€ записи
// arguments: char mode - символ, определ€ющий расширение файла:
// 'a' - архив
// 'f' - файл текстовый
// return: FILE* file - указатель на открытый файл
FILE* openFileForRecording(char);

// function: вывод размера файла
// arguments: FILE* file - указатель на файл
// return: uint32_t file_size - размер файла в  б
uint32_t fileSize(FILE*);

// function: архивации файла
// arguments: FILE* source - указатель на исходный файл
// return: FILE* archive - указатель на архив 
FILE* encoder(FILE*);

// function: распаковка файла
// arguments: FILE* encoder - указатель на сжатый файл
// return: FILE* file - указатель на текстовый файл 
FILE* decoder(FILE*);