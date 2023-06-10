#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "library.h"

int main(int argumentCounter, char const *arguments[])
{
    std::string inputFileName;            // name of the input file
    std::string outputFileName;           // name of the output file
    std::string dictionaryFileName;       // name of the dictionary file
    programMode _programMode = undefined; // program mode (undefined/compression/decompression)

    handleArguments(argumentCounter, arguments, inputFileName, outputFileName, dictionaryFileName, _programMode);

    if (_programMode == compression)
    {
        /* COMPRESSION */

        std::vector<char> text;                   // vector for storing characters read from the input file
        std::map<char, std::string> codes;        // map for storing unique character and its binary code
        std::map<char, unsigned int> frequencies; // map for storing unique character and its frequency
        std::string dictionary = "";              // special dictionary code
        node *root;                               // a pointer to the root node of the Huffman Tree
        int before = 0;                           // number of bits used before compression
        int after = 0;                            // number of bits used after compression

        readFile(inputFileName, text, frequencies);

        root = createHuffmanTree(frequencies);

        generateCodes(root, "", codes, dictionary);

        for (auto &&code : codes)
            std::cout << code.first << " == " << code.second << " | frequency == " << frequencies[code.first] << std::endl;

        printCompressedData(outputFileName, text, codes);

        generateDictionaryFile(dictionaryFileName, dictionary);

        before = text.size() * 8;
        for (auto &&i : frequencies)
            after += i.second * codes[i.first].length();

        std::cout << std::endl;
        std::cout << "Number of bits before compression == " << before << std::endl;
        std::cout << "Number of bits after compression == " << after << std::endl;
        std::cout << "Successful file compresion :D" << std::endl;
    }
    else
    {
        /* DECOMPRESSION */

        std::map<std::string, char> codes; // map for storing unique binary code and its character
        std::vector<char> binaryText;      // vector for storing bits read from the input file
        std::vector<char> text;            // vector for storing decompressed characters
        std::string code;                  // a binary code of the character

        readDictionaryFile(dictionaryFileName, codes);

        readBinaryFile(inputFileName, binaryText);
        for (auto &&bit : binaryText)
        {
            code += bit;
            if (codes.count(code))
            {
                text.push_back(codes[code]);
                code.clear();
            }
        }
        printDecompressedData(outputFileName, text);
        std::cout << "Successful file decompresion :D" << std::endl;
    }

    return 0;
}
