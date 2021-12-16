#define _CRT_SECURE_NO_WARNINGS
#include "ArchiverAndDearchiver.h"
#include <sys/types.h>
#include "dirent.h"

FILE* openFileForRecording(char mode) {
    FILE* file = NULL;
    char* file_path = (char*)malloc(UCHAR_MAX * sizeof(char));
    memset(file_path, 0, UCHAR_MAX);

    if (mode == 'a') printf("������� ���� � �������� ��� ���������� ������� �����:\n");
    else if (mode == 'f') printf("������� ���� � �������� ��� ���������� �������������� �����:\n");
    else return NULL;
    gets(file_path);
    if (file_path[strlen(file_path)] != '\\')
        strcat(file_path, "\\");

    if (mode == 'a') printf("������� ��� ������ (��� ����������):\n");
    else if (mode == 'f') printf("������� ��� ��� �������������� ����� (��� ����������):\n");
    char* file_name = (char*)malloc(INT8_MAX * sizeof(char));
    memset(file_name, 0, INT8_MAX);
    gets(file_name);
    strcat(file_path, file_name);
    free(file_name);

    if (mode == 'a') strcat(file_path, ".hfmn");
    else if (mode == 'f') strcat(file_path, ".txt");

    fopen_s(&file, file_path, "wb");
    if (file) {
        printf("\n���� ���������� � ������� ������.\n");
        printf("������ ���� � �����: %s\n", file_path);
        printf("������ �����: %d ��\n", fileSize(file));
    }
    free(file_path);

    return file;
}

uint32_t fileSize(FILE* file) {
    // ����� ������ ������ ����� �� 4��, ��� ��������� 4 ���� ����� � ���� � ���������� .txt ����� 3��
    fseek(file, 0, SEEK_END); // ������������� ��������� ����� � ����� �����
    uint32_t fileSizeBytes = ftell(file); // �������� ������� ��������� ��������� � ������
    fseek(file, 0, SEEK_SET); // ���������� ��������� �� ������ �����
    return (fileSizeBytes / 1024);
}

void archivingDirectory(char* basePath, const int root) {
    char* path = (char*)malloc(sizeof(char) * 1000);
    struct dirent* dp;
    DIR* dir = opendir(basePath);
    if (!dir) return;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy_s(path, sizeof(path), basePath);
            strcat_s(path, sizeof(path), "\\");
            strcat_s(path, sizeof(path), dp->d_name);
            printf("%s\n", path);
            //����� ����� ��������� ��������� ������, ������ ����� ���������� ����������
            archivingDirectory(path, root + 2);
        }
    }
    free(path);
    closedir(dir);
}

FILE* encoder(FILE* source) {

    //============================ ������ �������� ============================//

    // �������� ������� ������ ������������� ������ ���������� � �����
    uint32_t frequency_table[UINT8_MAX + 1] = { 0 };
    uint16_t number_dif_bytes = fillInFrequencyTable(source, frequency_table);
    // ������� �������� ������
    BinaryTree* tree = createBinaryTree(number_dif_bytes);
    // ��������� ������� ������ ����������� �� ����
    fillBinaryTree(tree, frequency_table);
    // ������ ������ ��������
    Node* root = buildHuffmanTree(tree, frequency_table, number_dif_bytes);

    //============================= ���� �������� =============================//

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0; uint8_t bits_in_code = 0;
    // �������� ������ ����� ��������
    getCodesForCompression(root, codes, code, bits_in_code);
    // ��� ������� ������������ ����� ����������� ����
    printCodes(codes);

    //============================ �������� ������ ============================//
 
    // ���������� ��������� ����� � ������ 
    fseek(source, 0, SEEK_SET);
    // ������� ���� ��� ������
    FILE* archive = openFileForRecording('a');
    if (!archive) return NULL;
    printf("������������ ������ �����, ����������, ���������...\n");

    //=================== ����� ���������� ��� ������������ ===================//

    // ���������� � �������� ���� ���������� ��������� ������ � �������� �����
    fwrite(&number_dif_bytes, sizeof(number_dif_bytes), 1, archive);
    // ����� �������� � �� ������
    for (uint16_t i = 0; i <= UINT8_MAX; ++i)
        if (frequency_table[i] != 0) {
            char input_byte = i + INT8_MIN;
            fread(&input_byte, sizeof(input_byte), 1, archive);
            fwrite(frequency_table + i, sizeof(uint32_t), 1, archive);
        }

    //============================== ������ ����� =============================//

    char input_byte = 0;
    uint64_t temp_code = 0;
    uint8_t output_byte = 0, remainder = 0;
    // ������� ����, �������� ����� �������� ����
    while (!feof(source)) {
        input_byte = fgetc(source); // ������ ���� �� ��������� �����
        temp_code = codes[input_byte - INT8_MIN].code; // ������� �������������� ��� ��������
        // ���������� ����� ����� �� ������� ���� ���� ��������
        temp_code <<= (sizeof(temp_code) * 8 - codes[input_byte - INT8_MIN].size);
        uint8_t bit_pos = remainder; // �������� �� �������� ����
        for (; bit_pos < codes[input_byte - INT8_MIN].size + remainder; bit_pos++) {
            // �� ����� �������� ������ ����� ��� ���� � ���������� ��� � ���� ��� ������
            output_byte = output_byte | ((temp_code & (UINT64_MAX - UINT64_MAX / 2)) ? 1 : 0);
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
    fputc(-1, archive); // EOF
    printf("���� ����!\n");
    printf("������ �����: %d ��\n", fileSize(archive));
    printf("����������� ������ �����: %4.3f\n", (float)fileSize(source) / (float)fileSize(archive));
    return archive;
}

FILE* decoder(FILE* archive) {

    //======================= ������ ���������� � ������ ======================//

    // �������� �� ������ ���������� ��������� ������ � �������� �����
    uint16_t number_dif_bytes;
    fread(&number_dif_bytes, sizeof(number_dif_bytes), 1, archive);

    // �������� ������� ������ ������������� ������ ����������
    uint32_t frequency_table[UINT8_MAX + 1] = { 0 };
    for (uint16_t i = 0; i < number_dif_bytes; i++) {
        char input_byte; uint32_t frequency;
        fread(&input_byte, sizeof(input_byte), 1, archive);
        fread(&frequency, sizeof(frequency), 1, archive);
        frequency_table[input_byte - INT8_MIN] = frequency;
    }

    //============================ ������ �������� ============================//

    // ������� �������� ������
    BinaryTree* tree = createBinaryTree(number_dif_bytes);
    // ��������� ������� ������ ����������� �� ����
    fillBinaryTree(tree, frequency_table);
    // ������ ������ ��������
    Node* root = buildHuffmanTree(tree, frequency_table, number_dif_bytes);

    //============================= ���� �������� =============================//

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0; uint8_t bits_in_code = 0;
    // �������� ������ ����� ��������
    getCodesForDecompression(root, codes, code, bits_in_code);
    // ��� ������� ������������ ����� ����������� ����
    printCodes(codes);
    // ���������� ���������� ����� �� �����, ��� ������� ���, ��� ���� �����������
    qsort(codes, number_dif_bytes, sizeof(HuffmanCode), compare2);

    // ������� ���� ��� ����������
    FILE* file = openFileForRecording('f');
    if (!file) return NULL;
    printf("������������ ���������� �����, ����������, ���������...\n");

    //============================ ���������� ����� ===========================//

    char input_byte = 0;
    uint64_t temp_code = 0;
    uint8_t output_byte = 0, remainder = 0;
    // ������� ����, �������� ����� �������� ����
    while (!feof(archive)) {
        input_byte = fgetc(archive); // ������ ���� �� ������
        for (uint8_t bit_pos = 0; bit_pos < CHAR_BIT; bit_pos++) {
            temp_code = input_byte & (UINT8_MAX - UINT8_MAX / 2);
            temp_code <<= 1;
            input_byte <<= 1;
            for (uint8_t index = 0; index < number_dif_bytes; index++)
                if (temp_code == codes[index].code) {
                    fputc(codes[index].byte, file);
                    temp_code = 0;
                    exit;
                }
        }
    }
    printf("���� ����������!\n");
    printf("������ �����: %d ��\n", fileSize(archive));

    return file;
}