#include "CommonHeader.h"
#include <sys/types.h>
#include "dirent.h"

FILE* openFile(char);
void fileSize(FILE*);
void tree(char*, const int);

void archiver(FILE*, FILE*, HuffmanCode[], uint16_t);