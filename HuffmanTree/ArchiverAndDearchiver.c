#define _CRT_SECURE_NO_WARNINGS
#include "ArchiverAndDearchiver.h"

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
            printf("������� ���� � �������� ��� ���������� ����� (������ ������������ \"\\\"):\n");
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
            printf("������� ���� � ����� ��� ������ ��� � ������ ��� ����������:\n");
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
    // ����� ������ ������ ����� �� 4��, ��� ��������� 4 ���� ����� � ���� � ���������� .txt ����� 3��
    fseek(file, 0, SEEK_END); // ������������� ��������� ����� � ����� �����
    uint32_t fileSizeBytes = ftell(file); // �������� ������� ��������� ��������� � ������
    fseek(file, 0, SEEK_SET); // ���������� ��������� �� ������ �����
    return (fileSizeBytes / 1024);
}

FILE* archiver(FILE* source) {

    //============================ ������ �������� ============================//

    // �������� ������� ������ ������������� ������ ���������� � �����
    uint64_t frequency_table[UINT8_MAX + 1] = { 0 };
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
    FILE* archive = openFile('w');
    printf("������������ ������ �����, ����������, ���������...\n");

    //=================== ����� ���������� ��� ������������ ===================//

    // ���������� � �������� ���� ���������� ��������� ������ � �������� �����
    fwrite(&number_dif_bytes, sizeof(number_dif_bytes), 1, archive);
    // ����� �������� � �� ������
    for (uint16_t i = 0; i <= UINT8_MAX; ++i)
        if (frequency_table[i] != 0) {
            fprintf(archive, "%c", i + INT8_MIN);
            fwrite(frequency_table + i, sizeof(uint64_t), 1, archive);
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

    //======================= ������ ���������� � ������ ======================//

    // �������� �� ������ ���������� ��������� ������ � �������� �����
    uint16_t number_dif_bytes = 0;
    fread(&number_dif_bytes, sizeof(number_dif_bytes), 1, archive);

    // �������� ������� ������ ������������� ������ ����������
    uint64_t frequency_table[UINT8_MAX + 1] = { 0 };
    for (uint16_t i = 0; i < number_dif_bytes; i++) {
        char input_byte = 0; uint64_t frequency = 0;
        fscanf(archive, "%c%llu ", &input_byte, &frequency);
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
    qsort(codes, number_dif_bytes, sizeof(HuffmanCode), compareDescending);

    // ������� ���� ��� ����������
    FILE* file = openFile('w');
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
                    exit;
                }
        }
    }
    //fputc(-1, archive); // EOF
    printf("���� ����������!\n");
    printf("������ �����: %d ��\n", fileSize(archive));

    fclose(file);
}