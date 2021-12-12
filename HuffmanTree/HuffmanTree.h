#include <locale.h>
#include <malloc.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char symbol;
    unsigned long freq;
    struct Node* left, * right;
} Node;
typedef struct BinaryTree {
    unsigned size;
    unsigned capacity;
    Node** array;
} BinaryTree;
typedef struct HuffmanCode {
    unsigned long code;
    unsigned char size;
} HuffmanCode;

unsigned fillInFrequencyTable(FILE*, unsigned[]);

Node* newNode(char, unsigned);
BinaryTree* createBinaryTree(unsigned);
void fillBinaryTree(BinaryTree*, unsigned[]);
Node* buildHuffmanTree(unsigned[], unsigned);

int isSizeOne(BinaryTree*);
int isLeaf(Node* root);
Node* extractNode(BinaryTree*, unsigned);
void insertNode(BinaryTree*, Node*, unsigned);
unsigned long compare(const void* a, const void* b);

FILE* openFile(char);
void getCodes(Node*, unsigned long[], unsigned long);
void printCodes(unsigned long[]);