#include "HuffmanTree.h"

uint16_t fillInFrequencyTable(FILE* file, uint64_t frequency_table[]) {
    char input_byte = 0; // переменная для временного хранения считанного байта информации
    uint16_t number_dif_byte = 0; // количество различных байтов в файле
    while (!feof(file)) {
        input_byte = fgetc(file);
        // если у считанный байт еще не встречался в файле
        if (frequency_table[input_byte - INT8_MIN] == 0)
            number_dif_byte++;
        frequency_table[input_byte - INT8_MIN]++;
    }
    return number_dif_byte;
}

Node* newNode(char byte, uint64_t freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->byte = byte;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BinaryTree* createBinaryTree(uint16_t capacity) {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    tree->size = 0; // изначальная заполненность дерева узлами равна нулю
    tree->capacity = capacity;
    tree->array = (Node**)malloc(tree->capacity * sizeof(Node*));
    return tree;
}

void fillBinaryTree(BinaryTree* tree, uint64_t frequency_table[]) {
    uint8_t j = 0; // счетчик для заполения массива дерева
    for (uint16_t i = 0; i <= UINT8_MAX; ++i)
        if (frequency_table[i] != 0) {
            // заполняем массив дерева попрядку
            tree->array[j++] = newNode((char)(i + INT8_MIN), frequency_table[i]);
            ++tree->size; // увеличиваем текущее количество узлов в массиве
        }
}

Node* buildHuffmanTree(uint64_t frequency_table[], uint16_t number_dif_char) {
    Node* left, * right, * top;
    // создаем бинарное дерево
    BinaryTree* tree = createBinaryTree(number_dif_char);
    // заполняем бираное дерево указателями на узлы
    fillBinaryTree(tree, frequency_table);
    // в цикле пока размер дерева не равен 1,
    // т.е. пока в дереве не останется только корень
    while (!isSizeOne(tree)) {
        // сортируем массив узлов дерева по возрастанию
        qsort(tree->array, tree->size, sizeof(Node*), compare);
        // извлекаем первый два узла с минимальными частотами встречаемости
        left = extractNode(tree, 0);
        right = extractNode(tree, 1);
        // создаем новый узел на основе двух извлеченных
        top = newNode('$', left->freq + right->freq);
        // делаем извлеченные узлы дочерними
        top->left = left;
        top->right = right;
        // вставляем новый узел в массив дерева
        insertNode(tree, top, 0);
        // перемечаем последний узел массива на образовавшееся пустое место
        insertNode(tree, extractNode(tree, --tree->size), 1);
    }
    //  после завершения цикла первым элементом в массиве будет корень дерева Хаффмана
    Node* root = extractNode(tree, 0);
    return root;
}

bool isSizeOne(BinaryTree* tree) {
    return (tree->size == 1);
}

bool isLeaf(Node* node) {
    return !(node->left) && !(node->right);
}

Node* extractNode(BinaryTree* tree, uint16_t index) {
    Node* Node = tree->array[index];
    // после извлечения стираем указатель на узел из массива
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
    // Функция необходима для производительного шифрования файла,
    // в ином случае для каждого символа приходилось бы каждый раз заново обходить дерево
    if (root->left)
        // рекурсивно вызываем функцию для левого узла
        getCodes(root->left, codes, (code << 1) | 0, number_of_bits + 1);
    if (root->right)
        // рекурсивно вызываем функцию для правого узла
        getCodes(root->right, codes, (code << 1) | 1, number_of_bits + 1);
    if (isLeaf(root)) { // если узел конечный, т.е. является листом
        codes[root->byte - INT8_MIN].code = code; // сохраняем код
        codes[root->byte - INT8_MIN].size = number_of_bits; // и количество битов в нем в массив
    }
}

void printCodes(HuffmanCode codes[]) {
    printf("\nХотите просмотреть коды Хаффмана?\nЧтобы пропустить, нажмите ESC.\n");
    printf("Для подтверждения нажмите любую другую клавишу.\n");
    if (_getch() != 27) { // функция _getch() возвращает код клавиши, 27 - код клавиши ESC
        system("cls");
        for (uint16_t i = 0; i <= UINT8_MAX; i++)
            if (codes[i].code != 0) {
                uint64_t code = codes[i].code; // используем временную переменную для вывода кода
                // выводим байт информации в десятичном виде и количество бит в коде Хаффмана
                printf("%4i | %2i | ", i + INT8_MIN, codes[i].size);
                for (uint16_t bit = 0; bit < sizeof(code) * 8; bit++) {
                    // пока не дойдем до кода Хаффмана, выводим точку
                    if (bit < sizeof(code) * 8 - codes[i].size) printf(".");
                    // после того, как продвинулись до кода, можем выводить биты, импользуя маску
                    else printf("%i", (code & (UINT64_MAX - (UINT64_MAX / 2))) ? 1 : 0);
                    code <<= 1; // производим логический сдвиг влево
                }
                printf("\n");
            }
        system("pause");
        system("cls");
    }
}