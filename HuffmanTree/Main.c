#include "ArchiverAndDearchiver.h"

// C:\Games\War and Society.txt
// C:\Games\aboba.txt

int main() {
    setlocale(LC_ALL, "Rus");
    FILE* source = openFile('r');
    FILE* archive = archiver(source);
    fseek(source, 0, SEEK_SET);
    dearchiver(archive);
    _fcloseall();
    return 0;
}