#define _CRT_SECURE_NO_WARNINGS
#include "ArchiverAndDearchiver.h"
#include "HuffmanTree.h"

#define getch _getch()
#define pause system("pause")
#define clear system("cls")
#define mask (UINT64_MAX - (UINT64_MAX / 2))

FILE* openFile(char mode) {
    FILE* file = NULL;
    char* file_path = (char*)malloc(UCHAR_MAX * sizeof(char));
    memset(file_path, 0, UCHAR_MAX);
    do {
        switch (mode) {
        case 'w':
            printf("������� ���� � �������� ��� ���������� ������ (������ ������������ \"\\\"):\n");
            gets(file_path);
            printf("������� ��� ������ ��� ����������:\n");
            char* file_name = (char*)malloc(INT8_MAX * sizeof(char));
            memset(file_name, 0, INT8_MAX);
            gets(file_name);
            strcat(file_path, file_name);
            strcat(file_path, ".hfmn");
            fopen_s(&file, file_path, "wb");
            break;
        case 'r':
        default:
            printf("������� ���� � ����� ��� ������������� ��� � ������ ��� ������������:\n");
            gets(file_path);
            fopen_s(&file, file_path, "rb");
            break;
        }
        if (file == NULL) {
            printf("���� �� ��������� ��� �� ��� ������! ���������� �����.");
            getch; clear;
        }
    } while (file == NULL);
    printf("\n���� ���������� � ������� ������.\n");
    printf("������ ���� � �����: %s\n", file_path);
    if (mode == 'r')
        printf("������ �����: %d ��\n", fileSize(file));
    free(file_path);
    return file;
}

uint32_t fileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    uint32_t fileSizeBytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    return (fileSizeBytes / 1024);
}

void tree(char* basePath, const int root) {
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
            //����� ����� ��������� ����������� �����
            tree(path, root + 2);
        }
    }
    closedir(dir);
}

void stringTraversalTree(FILE* archive, Node* root, char* str_traversal) {
    if (root->left) {
        // ������������� � ������ ������ ������ D, ��� ��������, ��� �� ����� �� ������ ����
        strcat(str_traversal, "D");
        // �������� ������� ��� ������ ���������
        stringTraversalTree(archive, root->left, str_traversal);
    }
    if (root->right) {
        // ������������� � ������ ������ ������ D, ��� ��������, ��� �� ����� �� ������ ����
        strcat(str_traversal, "D");
        // �������� ������� ��� ������� ���������
        stringTraversalTree(archive, root->right, str_traversal);
    }
    if (isLeaf(root)) {
        // ������������� � ������ ������ ������ U, ��� ��������, ��� �� ����� �� ������ �����
        strcat(str_traversal, "U");
        // ������� ���� ���������� �� ���� � �����
        fputc(root->byte, archive);
    }
}

FILE* archiver(FILE* source) {

    //============================ ������ �������� ============================//

    // �������� ������� ������ ������������� ������ ���������� � �����
    uint64_t frequency_table[UINT8_MAX + 1] = { 0 };
    uint16_t number_dif_bytes = fillInFrequencyTable(source, frequency_table);
    // ������ ������ ��������
    Node* root = buildHuffmanTree(frequency_table, number_dif_bytes);


    //============================= ���� �������� =============================//

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0; uint8_t bits_in_code = 0;
    // �������� ������ ����� ��������
    getCodes(root, codes, code, bits_in_code);
    // ��� ������� ������������ ����� ����������� ����
    printCodes(codes);

    //============================ �������� ������ ============================//
 
    // ���������� ��������� ����� � ������ 
    fseek(source, 0, SEEK_SET);
    // ������� ���� ��� ������
    FILE* archive = openFile('w');
    printf("������������ ������ �����, ����������, ���������...\n");


    //=================== ����� ���������� ��� ������������ ===================//

    // ���������� � �������� ���� ���������� ��������� ������ � �������� �����
    fwrite(number_dif_bytes, sizeof(number_dif_bytes), 1, archive);

    // ���������� � �������� ���� ����� ��� ������ ������ ������ � ������� � ������������ ������� ������ ������
    char* str_raversal = (char*)malloc(sizeof(char) * (UINT8_MAX + 1) * 4);
    memset(str_raversal, 0, (UINT8_MAX + 1) * 2);
    stringTraversalTree(archive, root, str_raversal);

    // ������� ���������� ��� ������ ������ ������
    fwrite((uint16_t)strcspn(str_raversal, "\0"), sizeof(uint16_t), 1, archive);

    // ������� ������ ������ ��������
    uint8_t output_byte = 0, remainder = 0;
    for (uint16_t i = 0; i < strcspn(str_raversal, "\0"); i++) {
        // �� ����� �������� ������ ����� ��� ���� � ���������� ��� � ���� ��� ������
        output_byte = output_byte | (str_raversal[i] == 'D' ? 1 : 0);
        output_byte <<= 1; // �������� ��� ������� �������
        if (i % CHAR_BIT == 0) // ���� ������� � ���� ������� ��� ������� �� 8
            fputc(output_byte, archive); // ������� ���� � �������� ����
    }

    //============================== ������ ����� =============================//

    char input_byte = 0;
    uint64_t temp_code = 0;
    // ������� ����, �������� ����� �������� ����
    while (!feof(source)) {
        input_byte = fgetc(source); // ������ ���� �� ��������� �����
        temp_code = codes[input_byte - INT8_MIN].code; // ������� �������������� ��� ��������
        // ���������� ����� ����� �� ������� ���� ���� ��������
        temp_code <<= (sizeof(temp_code) * 8 - codes[input_byte - INT8_MIN].size);
        uint8_t bit_pos = remainder; // �������� �� �������� ����
        for (; bit_pos < codes[input_byte - INT8_MIN].size + remainder; bit_pos++) {
            // �� ����� �������� ������ ����� ��� ���� � ���������� ��� � ���� ��� ������
            output_byte = output_byte | ((temp_code & mask) ? 1 : 0);
            output_byte <<= 1; // �������� ��� ������� �������
            if (bit_pos % CHAR_BIT == 0) // ������� � ���� �������� ������� ��� ������� �� 8
                fputc(output_byte, archive); // ������� ���� � �������� ����
                //fwrite(&output, sizeof(output), 1, archive);
        }
        remainder = bit_pos % CHAR_BIT; // ������� ������� �� �������� ����
    }
    if (remainder) { // ���� ������� ��������� ������������� �� ����� ����
        output_byte <<= 1; // �������� ��� � ������ �����
        fputc(output_byte, archive); // ������� ���
    }
    //fputc(-1, archive); // EOF
    printf("���� ����!\n");
    printf("������ �����: %d ��\n", fileSize(archive));
    printf("����������� ������ �����: %4.3f\n", (float)fileSize(source) / (float)fileSize(archive));
    return archive;
}

void dearchiver(FILE* archive) {

}