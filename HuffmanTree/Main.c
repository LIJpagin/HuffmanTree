#include "ArchiverAndDearchiver.h"

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
            printf("���� �� ��������� ��� �� ��� ������! ���������� �����.");
            system("pause");
            system("cls");
        }
        else {
            if (strstr(source_path, ".hfmn"))
                output = encoder(source);
            else if (strstr(source_path, ".txt"))
                output = decoder('r');
            else {
                fclose(source);
                source == NULL;
                printf("��������, �� �� �� ����� ������������ ����� �����(\n");
            }
            if (source) {
                printf("\n���� ���������� � ������� ������.\n");
                printf("������ ���� � �����: %s\n", source_path);
                printf("������ �����: %d ��\n", fileSize(source));
            }
        }
    } while (!source);
    if (!output) printf("���� �� ��� ������!");
    free(source_path);
    _fcloseall();
    return 0;
}