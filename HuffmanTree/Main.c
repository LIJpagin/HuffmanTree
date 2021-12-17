#include "ArchiverAndDearchiver.h"

// C:\tests\test1\War and Society.hfmn
// C:\tests\test2\Oblomov.hfmn
// C:\tests\test3\Kapitanskaya dochka.hfmn

int main() {
    setlocale(LC_ALL, "Rus");
    FILE* source = NULL;
    FILE* output = NULL;
    char* source_path = (char*)malloc(UCHAR_MAX * sizeof(char));
    memset(source_path, 0, UCHAR_MAX);
    do {
        printf("Введите полный путь файла для сжатия/архива для распаковки:\n");
        gets(source_path);
        fopen_s(&source, source_path, "rb");
        if (!source) {
            printf("Файл не обнаружен или не был открыт! Попробуйте снова.\n");
            system("pause");
            system("cls");
        }
        else {
            if (strstr(source_path, ".txt")) {
                printf("\nФайл успешно открыт для чтения.\n");
                printf("Полный путь к файлу: %s\n", source_path);
                printf("Размер файла: %d КБ\n", fileSize(source));
                output = encoder(source);
            }
            else if (strstr(source_path, ".hfmn")) {
                printf("\nФайл успешно открыт для чтения.\n");
                printf("Полный путь к файлу: %s\n", source_path);
                printf("Размер файла: %d КБ\n", fileSize(source));
                output = decoder('r');
            }
            else {
                fclose(source);
                source == NULL;
                printf("Простите, но мы не умеем архивировать такие файлы(\n");
            }
        }
    } while (!source);
    free(source_path);
    _fcloseall();
    return 0;
}