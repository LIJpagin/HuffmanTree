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
        printf("������� ������ ���� ����� ��� ������/������ ��� ����������:\n");
        gets(source_path);
        fopen_s(&source, source_path, "rb");
        if (!source) {
            printf("���� �� ��������� ��� �� ��� ������! ���������� �����.\n");
            system("pause");
            system("cls");
        }
        else {
            if (strstr(source_path, ".txt")) {
                printf("\n���� ������� ������ ��� ������.\n");
                printf("������ ���� � �����: %s\n", source_path);
                printf("������ �����: %d ��\n", fileSize(source));
                output = encoder(source);
            }
            else if (strstr(source_path, ".hfmn")) {
                printf("\n���� ������� ������ ��� ������.\n");
                printf("������ ���� � �����: %s\n", source_path);
                printf("������ �����: %d ��\n", fileSize(source));
                output = decoder('r');
            }
            else {
                fclose(source);
                source == NULL;
                printf("��������, �� �� �� ����� ������������ ����� �����(\n");
            }
        }
    } while (!source);
    free(source_path);
    _fcloseall();
    return 0;
}