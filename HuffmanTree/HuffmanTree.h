#include <stdbool.h>
#include <locale.h>
#include <malloc.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Node {
    char byte;
    uint32_t freq;
    struct Node* left, * right;
} Node;
typedef struct BinaryTree {
    uint16_t size;
    uint16_t capacity;
    Node** array;
} BinaryTree;
typedef struct HuffmanCode {
    char byte;
    uint64_t code;
    uint8_t size;
} HuffmanCode;

// function: заполнение таблицы частот встречаемости байтов информации
// arguments:
// FILE* file - указатель на файл для чтения
// uint32_t frequency_table[] - таблица частот встречаимости байт информации, заполненная определенным образом
// return: uint16_t number_dif_byte - количество различных байт в файле
uint16_t fillInFrequencyTable(FILE*, uint32_t[]);

// function: масштабирование частот встречаемости байтов в исходном файле
// arguments: uint32_t frequency_table[] - таблица частот встречаимости байт информации, заполненная определенным образом
void ScalingOfNodeFrequencies(uint32_t[]);

// function: создание нового узла
// arguments:
// char byte - байт информации из исходного файла
// uint32_t freq - частота вхождений байта в исходный текст
// return: Node* node - указатель на созданный узел
Node* newNode(char, uint32_t);

// function: создание бинарного дерева заданной вместимости
// arguments: uint16_t capacity - начальное количество узлов в дереве
// return: BinaryTree* tree - указатель на корень дерева
BinaryTree* createBinaryTree(uint16_t);

// function: заполнение бинароное дерева указателями на узлы
// arguments: 
// BinaryTree* tree - указатель на бинарное дерево
// uint32_t frequency_table[] - таблица частот встречаимости байт информации, заполненная определенным образом
void fillBinaryTree(BinaryTree*, uint32_t[]);

// function: построение дерева Хаффмана
// arguments:
// BinaryTree* tree - указатель на созданное банарное дерево
// uint32_t frequency_table[] - таблица частот встречаимости байтов информации, заполненная определенным образом
// uint16_t number_dif_char - количество различных байт в исходном файле
// return: Node* root - указатель на корневой элемент дерева Хаффмана
Node* buildHuffmanTree(BinaryTree*, uint32_t[], uint16_t);

// function: проверка на условие равенства единице размера бинарного дерева
// arguments: BinaryTree* tree - указатель на бинарное дерево
bool isSizeOne(BinaryTree*);

// function: проверка на условие конечный ли узел, т.е. является ли листом
// arguments: Node* node - указатель на узел
bool isLeaf(Node*);

// function: извлечение узла из бинарного дерева по индексу элемента массива
// arguments: 
// BinaryTree* tree - указатель на бинарное дерево
// uint16_t index - индекс элемента массива, из которого немобходимо извлечь узел
// return: Node* node - указатель на узел, извлеченный из бинарного дерева 
Node* extractNode(BinaryTree*, uint16_t);

// function: вставка узла в бинарное дерево по индексу элемента массива
// arguments: 
// BinaryTree* tree - указатель на бинарное дерево
// uint16_t index - индекс элемента массива, в который немобходимо вставить узел
// Node* node - указатель на узел для вставки в бинарное дерево
void insertNode(BinaryTree*, Node*, uint16_t);

// function: сравнение двух узлов по частоте встречаемости в исходном файле. Используется сортировкой qsort
// arguments:
// const void* a - первый указатель на элемент массива
// const void* b - второй указатель на элемент массива
// return: int32_t difference - разность частот встречаемости
int32_t compare1(const void*, const void*);

// function: сравнение длин кодов Хаффмана. Используется сортировкой qsort
// arguments:
// const void* a - первый указатель на элемент массива
// const void* b - второй указатель на элемент массива
// return: int8_t difference - разность длин кодов
uint8_t compare2(const void*, const void*);

// function: рекурсивная функция составления кодов Хаффмана прямым обходом дерева Хаффмана в глубину
// arguments:
// Node* root - указатель на корневой элемент дерева Хаффмана
// HuffmanCode codes[] - массив кодов Хаффмана, изначально пуст
// uint64_t code - переменная для записи кода при проходе по ветке, изначально равна 0
// uint8_t number_of_bits - количество бит в коде, изначально равно 0
void getCodesForCompression(Node*, HuffmanCode[], uint64_t, uint8_t);

// function: рекурсивая функция составления кодов Хаффмана прямым обходом дерева Хаффмана в глубину
// Отличается от getCodesForCompression способом заполнения массива кодов
// arguments:
// Node* root - указатель на корневой элемент дерева Хаффмана
// HuffmanCode codes[] - массив кодов Хаффмана, изначально пуст
// uint64_t code - переменная для записи кода при проходе по ветке, изначально равна 0
// uint8_t number_of_bits - количество бит в коде, изначально равно 0
void getCodesForDecompression(Node*, HuffmanCode[], uint64_t, uint8_t);

// function: вывод кодов Хаффмана в консоль по желанию пользователя
// arguments: HuffmanCode codes[] - массив кодов Хаффмана
void printCodes(HuffmanCode[]);