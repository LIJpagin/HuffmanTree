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
            printf("Введите путь к каталогу для сохранения архива (должен оканчиваться \"\\\"):\n");
            gets(file_path);
            printf("Введите имя архива без расширения:\n");
            char* file_name = (char*)malloc(INT8_MAX * sizeof(char));
            memset(file_name, 0, INT8_MAX);
            gets(file_name);
            strcat(file_path, "\\");
            strcat(file_path, file_name);
            strcat(file_path, ".hfmn");
            fopen_s(&file, file_path, "wb");
            break;
        case 'r':
        default:
            printf("Введите путь к файлу для архивирования или к архиву для разархивации:\n");
            gets(file_path);
            fopen_s(&file, file_path, "rb");
            break;
        }
        if (file == NULL) {
            printf("Файл не обнаружен или не был открыт! Попробуйте снова.");
            getch; clear;
        }
    } while (file == NULL);
    printf("\nФайл существует и успешно открыт.\n");
    printf("Полный путь к файлу: %s\n", file_path);
    if (mode == 'r')
        printf("Размер файла: %d КБ\n", fileSize(file));
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
            //ЗДЕСЬ МОЖНО ЗАПУСКАТЬ КОДИРОВАНИЕ ФАЙЛА
            tree(path, root + 2);
        }
    }
    closedir(dir);
}

void stringTraversalTree(Node* root, uint64_t str_traversal) {

}

FILE* archiver(FILE* source) {
    uint64_t frequency_table[UINT8_MAX + 1] = { 0 };
    // получаем таблицу частот встречаемости байтов информации в файле
    uint16_t number_dif_char = fillInFrequencyTable(source, frequency_table);
    // строим дерево Хаффмана
    Node* root = buildHuffmanTree(frequency_table, number_dif_char);

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0; uint8_t number_of_bits = 0;
    // получаем массив кодов Хаффмана
    getCodes(root, codes, code, number_of_bits);
    // при желании пользователь может просмотреть коды
    printCodes(codes);

    // возвращаем указатель файла в начало 
    fseek(source, 0, SEEK_SET);
    // создаем файл для архива
    FILE* archive = openFile('w');

    printf("Производится сжатие файла, пожалуйста, подождите...\n");
    char input_byte = 0;
    uint8_t output_byte = 0, remainder = 0;
    uint64_t temp_code = 0;
    // сжимаем файл, побайтно читая исходный файл
    while (!feof(source)) {
        input_byte = fgetc(source); // читаем байт из исходного файла
        temp_code = codes[input_byte - INT8_MIN].code; // находим соответсвующий код Хаффмана
        // производим сдвиг влево до первого бита кода Хаффмана
        temp_code <<= (sizeof(temp_code) * 8 - codes[input_byte - INT8_MIN].size);
        uint8_t bit_pos = remainder; // отстаток от прошлого кода Хаффмана
        for (; bit_pos < codes[input_byte - INT8_MIN].size + remainder; bit_pos++) {
            // по маске выделяем первый слева бит кода и записываем его в байт для вывода
            output_byte = output_byte | ((temp_code & mask) ? 1 : 0);
            output_byte <<= 1; // сдвигаем код влевона единицу
            if (bit_pos % CHAR_BIT == 0) // позиция в коде Хаффмана делится без остатка на 8
                fputc(output_byte, archive); // выводим байт в выходной файл
                //fwrite(&output, sizeof(output), 1, archive);
        }
        remainder = bit_pos % CHAR_BIT; // находим остаток от прошлого кода
    }
    if (remainder) { // если остался последний незаполненный до конца байт
        output_byte <<= 1; // сдвигаем код в начало байта
        fputc(output_byte, archive); // выводим его
    }
    fputc(-1, archive); // EOF
    printf("Файл сжат!\n");
    printf("Размер файла: %d КБ\n", fileSize(archive));
    printf("Коэффициент сжатия файла: %4.3f\n", (float)fileSize(source) / (float)fileSize(archive));
    return archive;
}

void dearchiver(FILE* archive) {

}