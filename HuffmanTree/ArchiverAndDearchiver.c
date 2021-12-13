#include "ArchiverAndDearchiver.h"
#include "HuffmanTree.h"

#define getch _getch()
#define pause system("pause")
#define clear system("cls")

FILE* openFile(char mode) {
    FILE* file = NULL;
    char* file_path = (char*)malloc(UCHAR_MAX * sizeof(char));
    memset(file_path, NULL, UCHAR_MAX);
    do {
        switch (mode) {
        case 'w':
            printf("Введите путь и имя для сохранения архива:\n");
            break;
        case 'r':
        default:
            printf("Введите путь к файлу для архивирования или к архиву для разархивации:\n");
            break;
        }
        gets(file_path);
        fopen_s(&file, file_path, "r");
        if (file == NULL) {
            printf("Файл не обнаружен или не был открыт! Попробуйте снова.");
            getch; clear;
        }
    } while (file == NULL);
    printf("Файл существует и успешно открыт.\n");
    free(file_path);
    return file;
}

void fileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    unsigned long fileSizeBytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("Размер файла: %d КБ\n", fileSizeBytes / 1024);
}

void tree(char* basePath, const int root) {
    int i;
    char path[1000];
    struct dirent* dp;
    DIR* dir = opendir(basePath);
    if (!dir) return;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy_s(path, sizeof(path), basePath);
            strcat_s(path, sizeof(path), "\\");
            strcat_s(path, sizeof(path), dp->d_name);
            printf("%s\n", path);
            //ЗДЕСЬ МОЖНО ЗАПУСКАТЬ КОДИРОВАНИЕ ФАЙЛА
            tree(path, root + 2);
        }
    }
    closedir(dir);
}

void stringTraversalTree(Node* root) {
    if (root->left) {
        printf("0");
        stringTraversalTree(root->left);
    }
    if (root->right) {
        printf("1");
        stringTraversalTree(root->right);
    }
    if (isLeaf(root)) 
        printf("%c", root->symbol);
}

void archiver(FILE* source, FILE* archive, HuffmanCode codes[], uint16_t number_dif_char) {

}