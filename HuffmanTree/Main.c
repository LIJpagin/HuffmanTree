#include "HuffmanTree.h"

// C:\\Games\\War and Society.txt

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