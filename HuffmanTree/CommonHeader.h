#pragma once
#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

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