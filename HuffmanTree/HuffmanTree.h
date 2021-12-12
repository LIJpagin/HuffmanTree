#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Node {
    char symbol;
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

uint16_t fillInFrequencyTable(FILE*, uint64_t[]);

Node* newNode(char, uint64_t);
BinaryTree* createBinaryTree(uint16_t);
void fillBinaryTree(BinaryTree*, uint64_t[]);
Node* buildHuffmanTree(uint64_t[], uint16_t);

bool isSizeOne(BinaryTree*);
bool isLeaf(Node*);
Node* extractNode(BinaryTree*, uint16_t);
void insertNode(BinaryTree*, Node*, uint16_t);
int64_t compare(const void*, const void*);

FILE* openFile(char);
void getCodes(Node*, HuffmanCode[], uint64_t, char);
void printCodes(HuffmanCode[]);