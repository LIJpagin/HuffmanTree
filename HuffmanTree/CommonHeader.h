#pragma once
#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct HuffmanCode {
    uint64_t code;
    uint8_t size;
} HuffmanCode;