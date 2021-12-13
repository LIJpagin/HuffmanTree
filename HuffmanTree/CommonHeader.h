#pragma once
#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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