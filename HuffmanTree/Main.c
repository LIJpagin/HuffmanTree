#include "HuffmanTree.h"
#include "ArchiverAndDearchiver.h"

// C:\\Games\\War and Society.txt
// C:\\Games\\aboba.txt

int main() {
    setlocale(LC_ALL, "Rus");
    FILE* source = openFile('r');

    uint64_t frequency_table[UINT8_MAX + 1] = { 0 };
    uint16_t number_dif_char = fillInFrequencyTable(source, frequency_table);
    Node* root = buildHuffmanTree(frequency_table, number_dif_char);

    HuffmanCode codes[UINT8_MAX + 1] = { 0 };
    uint64_t code = 0;
    uint8_t number_of_bits = 0;
    getCodes(root, codes, code, number_of_bits);
    printCodes(codes);

    FILE* archive = openFile('w');

    archiver(source, archive, codes, number_dif_char);

    return 0;
}