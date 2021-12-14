#include "HuffmanTree.h"

uint16_t fillInFrequencyTable(FILE* file, uint64_t frequency_table[]) {
    char symbol = '\0';
    uint16_t number_dif_char = 0;
    while (!feof(file)) {
        symbol = fgetc(file);
        if (frequency_table[(int)symbol - INT8_MIN] == 0)
            number_dif_char++;
        frequency_table[(int)symbol - INT8_MIN]++;
    }
    return number_dif_char;
}

Node* newNode(char symbol, uint64_t freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->symbol = symbol;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BinaryTree* createBinaryTree(uint16_t capacity) {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    tree->size = 0;
    tree->capacity = capacity;
    tree->array = (Node**)malloc(tree->capacity * sizeof(Node*));
    return tree;
}

void fillBinaryTree(BinaryTree* tree, uint64_t frequency_table[]) {
    uint8_t j = 0;
    for (uint16_t i = 0; i <= UINT8_MAX; ++i)
        if (frequency_table[i] != 0) {
            tree->array[j++] = newNode((char)(i + INT8_MIN), frequency_table[i]);
            ++tree->size;
        }
}

Node* buildHuffmanTree(uint64_t frequency_table[], uint16_t number_dif_char) {
    Node* left, * right, * top;
    BinaryTree* tree = createBinaryTree(number_dif_char);
    fillBinaryTree(tree, frequency_table);
    while (!isSizeOne(tree)) {
        qsort(tree->array, tree->size, sizeof(Node*), compare);
        left = extractNode(tree, 0);
        right = extractNode(tree, 1);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertNode(tree, top, 0);
        insertNode(tree, extractNode(tree, --tree->size), 1);
    }
    Node* root = extractNode(tree, 0);
    return root;
}

bool isSizeOne(BinaryTree* tree) {
    return (tree->size == 1);
}

bool isLeaf(Node* root) {
    return !(root->left) && !(root->right);
}

Node* extractNode(BinaryTree* tree, uint16_t index) {
    Node* Node = tree->array[index];
    tree->array[index] = NULL;
    return Node;
}

void insertNode(BinaryTree* tree, Node* node, uint16_t index) {
    tree->array[index] = node;
}

int64_t compare(const void* a, const void* b) {
    const Node* const* aa = a;
    const Node* const* bb = b;
    return (*aa)->freq - (*bb)->freq;
}

void getCodes(Node* root, HuffmanCode codes[], uint64_t code, uint8_t number_of_bits) {
    if (root->left)
        getCodes(root->left, codes, (code << 1) | 0, number_of_bits + 1);
    if (root->right)
        getCodes(root->right, codes, (code << 1) | 1, number_of_bits + 1);
    if (isLeaf(root)) {
        codes[(int)root->symbol - INT8_MIN].code = code;
        codes[(int)root->symbol - INT8_MIN].size = number_of_bits;
    }
}

void printCodes(HuffmanCode codes[]) {
    for (uint16_t i = 0; i <= UINT8_MAX; i++)
        if (codes[i].code != 0) {
#if false
            printf("%c | %13ld | %4i\n", i + INT8_MIN, codes[i].code, codes[i].size);
#else
            uint64_t code = codes[i].code;
            printf("%4i | %2i | ", i + INT8_MIN, codes[i].size);
            for (int bit = 0; bit < sizeof(code) * 8; bit++) {
                if (bit < sizeof(code) * 8 - codes[i].size) printf(".");
                else printf("%i", (code & (UINT64_MAX - (UINT64_MAX / 2))) ? 1 : 0);
                code <<= 1;
            }
            printf("\n");
#endif
        }
}