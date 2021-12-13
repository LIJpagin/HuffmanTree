#include "CommonHeader.h"

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
void getCodes(Node*, HuffmanCode[], uint64_t, uint8_t);
void printCodes(HuffmanCode[]);