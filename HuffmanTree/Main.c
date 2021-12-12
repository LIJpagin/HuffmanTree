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

Node* newNode(char, unsigned);
BinaryTree* createBinaryTree(unsigned);
void fillBinaryTree(BinaryTree*, unsigned[]);
Node* buildHuffmanTree(unsigned[], unsigned);
int isSizeOne(BinaryTree*);
int isLeaf(Node* root);
Node* extractNode(BinaryTree*, unsigned);
void insertNode(BinaryTree*, Node*, unsigned);
long compare(const void* a, const void* b);
unsigned fillInFrequencyTable(FILE*, unsigned[]);
FILE* openFile(char);
void getCodes(Node*, unsigned long[], unsigned long);
void printCodes(unsigned long[]);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main() {
    setlocale(LC_ALL, "Rus");
    FILE* file = openFile('r');

    unsigned long frequency_table[UCHAR_MAX] = { 0 };
    unsigned number_dif_char = fillInFrequencyTable(file, frequency_table);
    Node* root = buildHuffmanTree(frequency_table, number_dif_char);

    HuffmanCode codes[UCHAR_MAX + 1] = { 0 };
    unsigned long code = 0;
    unsigned char number_of_bits = 0;
    getCodes(root, codes, code, number_of_bits);
    printCodes(codes);

    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Node* newNode(char symbol, unsigned freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->symbol = symbol;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BinaryTree* createBinaryTree(unsigned capacity) {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    tree->size = 0;
    tree->capacity = capacity;
    tree->array = (Node*)malloc(tree->capacity * sizeof(Node*));
    return tree;
}

void fillBinaryTree(BinaryTree* tree, unsigned frequency_table[]) {
    unsigned j = 0;
    for (unsigned i = 0; i <= UCHAR_MAX; ++i)
        if (frequency_table[i] != 0) {
            tree->array[j++] = newNode((char)(i + SCHAR_MIN), frequency_table[i]);
            tree->size++;
        }
}

Node* buildHuffmanTree(unsigned frequency_table[], unsigned number_dif_char) {
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

int isSizeOne(BinaryTree* tree) {
    return (tree->size == 1);
}

int isLeaf(Node* root) {
    return !(root->left) && !(root->right);
}

Node* extractNode(BinaryTree* tree, unsigned index) {
    Node* Node = tree->array[index];
    tree->array[index] = NULL;
    return Node;
}

void insertNode(BinaryTree* tree, Node* node, unsigned index) {
    tree->array[index] = node;
}

long compare(const void* a, const void* b) {
    long freqA = ((Node*)a)->freq;
    long freqB = ((Node*)b)->freq;
    if (!freqA) return freqB;
    if (!freqB) return -freqA;
    return freqA - freqB;
}

unsigned fillInFrequencyTable(FILE* file, unsigned frequency_table[]) {
    char symbol = '\0';
    unsigned number_dif_char = 0;
    while (!feof(file)) {
        symbol = fgetc(file);
        if (frequency_table[(int)symbol + SCHAR_MAX] == 0)
            number_dif_char++;
        frequency_table[(int)symbol + SCHAR_MAX]++;
    }
    return number_dif_char;
}

FILE* openFile(char mode) {
    FILE* file = NULL;
    char* file_path = (char*)malloc(UCHAR_MAX * sizeof(char));
    memset(file_path, NULL, UCHAR_MAX);
    do {
        switch (mode) {
        case 'w':
            printf("Введите путь и имя для сохранения архива:\n");
            break;
        case 'r':
        default:
            printf("Введите путь к файлу для архивирования или к архиву для разархивации:\n");
            break;
        }
        gets(file_path);
        fopen_s(&file, file_path, "r");
        if (file == NULL)
            printf("Файл не обнаружен или не был открыт! Попробуйте снова.\n");
    } while (file == NULL);
    printf("Файл существует и успешно открыт.\n");
    free(file_path);
    return file;
}

void getCodes(Node* root, HuffmanCode codes[], unsigned long code, char number_of_bits) {
    if (root->left) {
        code = (code << 1) | 0;
        number_of_bits++;
        getCodes(root->left, codes, code, number_of_bits);
    }
    if (root->right) {
        code = (code << 1) | 1;
        number_of_bits++;
        getCodes(root->right, codes, code, number_of_bits);
    }
    if (isLeaf(root)) {
        codes[(int)root->symbol + SCHAR_MAX].code = code;
        codes[(int)root->symbol + SCHAR_MAX].size = number_of_bits;
    }
}

void printCodes(HuffmanCode codes[]) {
    for (int i = 0; i <= UCHAR_MAX; i++)
        if (codes[i].code != 0) {
            printf("%4i| %13i| %4i\n", i - SCHAR_MAX, codes[i].code, codes[i].size);
            //unsigned long code = codes[i].code;
            //printf("%4i: ", i - SCHAR_MAX);
            //for (int bit = 0; bit < sizeof(code) * 8 - codes[i].size; bit++) {
            //    printf(" ");
            //    code <<= 1;
            //}
            //for (int bit = 0; bit < codes[i].size; bit++) {
            //    printf("%c", (code & (ULONG_MAX - (ULONG_MAX / 2))) ? '1' : '0');
            //    code <<= 1;
            //}
            //printf("\n");
        }
}
// C:\\Games\\War and Society.txt

// C:\\Users\\Пользователь\\Desktop\\aboba.txt