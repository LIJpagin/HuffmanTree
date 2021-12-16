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
    uint64_t freq;
    struct Node* left, * right;
} Node;
typedef struct BinaryTree {
    uint16_t size;
    uint16_t capacity;
    Node** array;
} BinaryTree;
typedef struct HuffmanCode {
    uint64_t code;
    uint8_t size;
} HuffmanCode;

// function: ���������� ������� ������ ������������� ������ ����������
// arguments:
// FILE* file - ��������� �� ���� ��� ������
// uint64_t frequency_table[] - ������� ������ ������������� ������ ����������, ����������� ������������ �������
// return: uint16_t number_dif_byte - ���������� ��������� ������ � �����
uint16_t fillInFrequencyTable(FILE*, uint64_t[]);

// function: ��������������� ������ ������������� ������ � �������� �����
// arguments: uint64_t frequency_table[] - ������� ������ ������������� ������ ����������, ����������� ������������ �������
void ScalingOfNodeFrequencies(uint64_t[]);

// function: �������� ������ ����
// arguments:
// char byte - ���� ���������� �� ��������� �����
// uint64_t freq - ������� ��������� ����� � �������� �����
// return: Node* node - ��������� �� ��������� ����
Node* newNode(char, uint64_t);

// function: �������� ��������� ������ �������� �����������
// arguments: uint16_t capacity - ��������� ���������� ����� � ������
// return: BinaryTree* tree - ��������� �� ������ ������
BinaryTree* createBinaryTree(uint16_t);

// function: ���������� ��������� ������ ����������� �� ����
// arguments: 
// BinaryTree* tree - ��������� �� �������� ������
// uint64_t frequency_table[] - ������� ������ ������������� ������ ����������, ����������� ������������ �������
void fillBinaryTree(BinaryTree*, uint64_t[]);

// function: ���������� ������ ��������
// arguments:
// BinaryTree* tree - ��������� �� ��������� �������� ������
// uint64_t frequency_table[] - ������� ������ ������������� ������ ����������, ����������� ������������ �������
// uint16_t number_dif_char - ���������� ��������� ���� � �������� �����
// return: Node* root - ��������� �� �������� ������� ������ ��������
Node* buildHuffmanTree(BinaryTree*, uint64_t[], uint16_t);

// function: �������� �� ������� ��������� ������� ������� ��������� ������
// arguments: BinaryTree* tree - ��������� �� �������� ������
bool isSizeOne(BinaryTree*);

// function: �������� �� ������� �������� �� ����, �.�. �������� �� ������
// arguments: Node* node - ��������� �� ����
bool isLeaf(Node*);

// function: ���������� ���� �� ��������� ������ �� ������� �������� �������
// arguments: 
// BinaryTree* tree - ��������� �� �������� ������
// uint16_t index - ������ �������� �������, �� �������� ����������� ������� ����
// return: Node* node - ��������� �� ����, ����������� �� ��������� ������ 
Node* extractNode(BinaryTree*, uint16_t);

// function: ������� ���� � �������� ������ �� ������� �������� �������
// arguments: 
// BinaryTree* tree - ��������� �� �������� ������
// uint16_t index - ������ �������� �������, � ������� ����������� �������� ����
// Node* node - ��������� �� ���� ��� ������� � �������� ������
void insertNode(BinaryTree*, Node*, uint16_t);

// function: ��������� ���� ����� �� ������� ������������� � �������� �����. ������������ ����������� qsort
// arguments:
// const void* a - ������ ��������� �� ������� �������
// const void* b - ������ ��������� �� ������� �������
// return: int64_t difference - �������� ������ �������������
int64_t compare(const void*, const void*);

// function: ���������� ������� ����������� ����� �������� ������� ������ ��������
// arguments:
// Node* root - ��������� �� �������� ������� ������ ��������
// HuffmanCode codes[] - ������ ����� ��������, ���������� ����
// uint64_t code - ���������� ��� ������ ���� ��� ������� �� �����, ���������� ����� 0
// uint8_t number_of_bits - ���������� ��� � ����, ���������� ����� 0
void getCodes(Node*, HuffmanCode[], uint64_t, uint8_t);

// function: ����� ����� �������� � ������� �� ������� ������������
// arguments: // HuffmanCode codes[] - ������ ����� ��������
void printCodes(HuffmanCode[]);