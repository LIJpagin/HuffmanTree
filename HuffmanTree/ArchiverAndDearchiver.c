#define _CRT_SECURE_NO_WARNINGS
#include "ArchiverAndDearchiver.h"
#include <sys/types.h>
#include "dirent.h"

FILE* openFileForRecording(char mode) {
    FILE* file = NULL;
    char* file_path = (char*)malloc(UCHAR_MAX * sizeof(char));
    memset(file_path, 0, UCHAR_MAX);

    if (mode == 'a') printf("Введите путь к каталогу для сохранения сжатого файла:\n");
    else if (mode == 'f') printf("Введите путь к каталогу для сохранения распакованного файла:\n");
    else return NULL;
    gets(file_path);
    if (file_path[strlen(file_path)] != '\\')
        strcat(file_path, "\\");

    if (mode == 'a') printf("Введите имя архива (без расширения):\n");
    else if (mode == 'f') printf("Введите имя для распакованного файла (без расширения):\n");
    char* file_name = (char*)malloc(INT8_MAX * sizeof(char));
    memset(file_name, 0, INT8_MAX);
    gets(file_name);
    strcat(file_path, file_name);
    free(file_name);

    if (mode == 'a') strcat(file_path, ".hfmn");
    else if (mode == 'f') strcat(file_path, ".txt");

    fopen_s(&file, file_path, "wb");
    if (file) {
        printf("\nФайл существует и успешно открыт.\n");
        printf("Полный путь к файлу: %s\n", file_path);
        printf("Размер файла: %d КБ\n", fileSize(file));
    }
    free(file_path);

    return file;
}

uint32_t fileSize(FILE* file) {
    // можно узнать размер файла до 4гб, для сравнения 4 тома войны и мира в расширении .txt весят 3Мб
    fseek(file, 0, SEEK_END); // установливаем указатель файла в конец файла
    uint32_t fileSizeBytes = ftell(file); // получаем текущее положение указателя в байтах
    fseek(file, 0, SEEK_SET); // возвращаем указатель на начало файла
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
            //ЗДЕСЬ МОЖНО ЗАПУСКАТЬ АРХИВАЦИЮ ФАЙЛОВ, ОДНАКО НУЖНО ПЕРЕПИСАТЬ КОДИРОВЩИК
            archivingDirectory(path, root + 2);
        }
    }
    free(path);
    closedir(dir);
}

FILE* encoder(FILE* source) {

    //============================ ДЕРЕВО ХАФФМАНА ============================//

    // получаем таблицу частот встречаемости байтов информации в файле
    uint32_t frequency_table[UINT8_MAX + 1] = { 0 };
    uint16_t number_dif_bytes = fillInFrequencyTable(source, frequency_table);
    // создаем бинарное дерево
    BinaryTree* tree = createBinaryTree(number_dif_bytes);
    // заполняем бираное дерево указателями на узлы
    fillBinaryTree(tree, frequency_table);
    // строим дерево Хаффмана
    Node* root = buildHuffmanTree(tree, frequency_table, number_dif_bytes);

    //============================= КОДЫ ХАФФМАНА =============================//

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0; uint8_t bits_in_code = 0;
    // получаем массив кодов Хаффмана
    getCodesForCompression(root, codes, code, bits_in_code);
    // при желании пользователь может просмотреть коды
    printCodes(codes);

    //============================ СОЗДАНИЕ АРХИВА ============================//
 
    // возвращаем указатель файла в начало 
    fseek(source, 0, SEEK_SET);
    // создаем файл для архива
    FILE* archive = openFileForRecording('a');
    if (!archive) return NULL;
    printf("Производится сжатие файла, пожалуйста, подождите...\n");

    //=================== ВЫВОД ИНФОРМАЦИИ ДЛЯ ДЕАРХИВАТОРА ===================//

    // записываем в выходной файл количество различных байтов в исходном файле
    fwrite(&number_dif_bytes, sizeof(number_dif_bytes), 1, archive);
    // вывод символов и их частот
    for (uint16_t i = 0; i <= UINT8_MAX; ++i)
        if (frequency_table[i] != 0) {
            char input_byte = i + INT8_MIN;
            fread(&input_byte, sizeof(input_byte), 1, archive);
            fwrite(frequency_table + i, sizeof(uint32_t), 1, archive);
        }

    //============================== СЖАТИЕ ФАЙЛА =============================//

    char input_byte = 0;
    uint64_t temp_code = 0;
    uint8_t output_byte = 0, remainder = 0;
    // сжимаем файл, побайтно читая исходный файл
    while (!feof(source)) {
        input_byte = fgetc(source); // читаем байт из исходного файла
        temp_code = codes[input_byte - INT8_MIN].code; // находим соответсвующий код Хаффмана
        // производим сдвиг влево до первого бита кода Хаффмана
        temp_code <<= (sizeof(temp_code) * 8 - codes[input_byte - INT8_MIN].size);
        uint8_t bit_pos = remainder; // отстаток от прошлого кода
        for (; bit_pos < codes[input_byte - INT8_MIN].size + remainder; bit_pos++) {
            // по маске выделяем первый слева бит кода и записываем его в байт для вывода
            output_byte = output_byte | ((temp_code & (UINT64_MAX - UINT64_MAX / 2)) ? 1 : 0);
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

FILE* decoder(FILE* archive) {

    //======================= ЧТЕНИЕ ИНФОРМАЦИИ О СЖАТИИ ======================//

    // получаем из архива количество различных байтов в исходном файле
    uint16_t number_dif_bytes;
    fread(&number_dif_bytes, sizeof(number_dif_bytes), 1, archive);

    // получаем таблицу частот встречаемости байтов информации
    uint32_t frequency_table[UINT8_MAX + 1] = { 0 };
    for (uint16_t i = 0; i < number_dif_bytes; i++) {
        char input_byte; uint32_t frequency;
        fread(&input_byte, sizeof(input_byte), 1, archive);
        fread(&frequency, sizeof(frequency), 1, archive);
        frequency_table[input_byte - INT8_MIN] = frequency;
    }

    //============================ ДЕРЕВО ХАФФМАНА ============================//

    // создаем бинарное дерево
    BinaryTree* tree = createBinaryTree(number_dif_bytes);
    // заполняем бираное дерево указателями на узлы
    fillBinaryTree(tree, frequency_table);
    // строим дерево Хаффмана
    Node* root = buildHuffmanTree(tree, frequency_table, number_dif_bytes);

    //============================= КОДЫ ХАФФМАНА =============================//

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0; uint8_t bits_in_code = 0;
    // получаем массив кодов Хаффмана
    getCodesForDecompression(root, codes, code, bits_in_code);
    // при желании пользователь может просмотреть коды
    printCodes(codes);
    // производим сортировку кодов по длине, чем длиннее код, тем реже встречается
    qsort(codes, number_dif_bytes, sizeof(HuffmanCode), compare2);

    // создаем файл для распаковки
    FILE* file = openFileForRecording('f');
    if (!file) return NULL;
    printf("Производится распаковка файла, пожалуйста, подождите...\n");

    //============================ РАСПАКОВКА ФАЙЛА ===========================//

    char input_byte = 0;
    uint64_t temp_code = 0;
    uint8_t output_byte = 0, remainder = 0;
    // сжимаем файл, побайтно читая исходный файл
    while (!feof(archive)) {
        input_byte = fgetc(archive); // читаем байт из архива
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
    printf("Файл распакован!\n");
    printf("Размер файла: %d КБ\n", fileSize(archive));

    return file;
}