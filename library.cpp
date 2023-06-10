#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <bitset>
#include "library.h"

void handleArguments(int argumentCounter,
                     char const *arguments[],
                     std::string &inputFileName,
                     std::string &outputFileName,
                     std::string &dictionaryFileName,
                     programMode &_programMode)
{
    if (argumentCounter <= 1)
        throwError("No arguments.");

    for (int i = 1; i < argumentCounter; i++)
    {
        std::string argument(arguments[i]);
        if (argument == "-i")
        {
            i++;
            if (i == argumentCounter)
                throwError("Input file is missing.");
            inputFileName = arguments[i];
            continue;
        }

        if (argument == "-o")
        {
            i++;
            if (i == argumentCounter)
                throwError("Output file is missing.");
            outputFileName = arguments[i];
            continue;
        }

        if (argument == "-m")
        {
            i++;
            if (i == argumentCounter)
                throwError("Program mode is missing.");
            std::string mode(arguments[i]);
            if (mode == "c")
            {
                _programMode = compression;
                continue;
            }
            else if (mode == "d")
            {
                _programMode = decompression;
                continue;
            }
            else
                throwError("Invalid program mode.");
        }

        if (argument == "-d")
        {
            i++;
            if (i == argumentCounter)
                throwError("Dictionary file missing.");
            dictionaryFileName = arguments[i];
            continue;
        }

        std::cerr << "Unknown argument: " << argument << std::endl;
    }
    if (inputFileName.empty())
        throwError("Input file is missing.");
    if (outputFileName.empty())
        throwError("Output file is missing.");
    if (dictionaryFileName.empty())
        throwError("Dictionary file is missing.");
    if (!_programMode)
        throwError("Program mode is missing.");
}

void throwError(const std::string &error)
{
    std::cerr << std::endl
              << error
              << std::endl
              << std::endl
              << "Compress or decompress file with Huffmann's method."
              << std::endl
              << std::endl
              << "Type arguments in the following format to compress the file:"
              << std::endl
              << "-i input_file_name -o output_file_name -m c -d dictionary_file_name"
              << std::endl
              << std::endl
              << "Type arguments in the following format to decompress the file:"
              << std::endl
              << "-i input_file_name -o output_file_name -m d -d dictionary_file_name"
              << std::endl;
    ::exit(1);
}

node *findMinimumNode(std::vector<node *> &nodeSet)
{
    node *minNode = nodeSet.front();
    int minNodeIndex = 0; // an index of the node with lowest frequency
    for (int i = 1; i < nodeSet.size(); i++)
        if (nodeSet[i]->frequency < minNode->frequency)
        {
            minNode = nodeSet[i];
            minNodeIndex = i;
        }
    nodeSet.erase(nodeSet.begin() + minNodeIndex);
    return minNode;
}

node *createHuffmanTree(std::map<char, unsigned int> &frequencies)
{
    node *left, *right, *parent; // pointers to left child node, right child node and parent node
    std::vector<node *> heap;    // set of all pointers to nodes
    for (auto &&character : frequencies)
        heap.push_back(new node(character.first, character.second));

    while (heap.size() != 1)
    {
        left = findMinimumNode(heap);

        right = findMinimumNode(heap);

        parent = new node(-1, left->frequency + right->frequency);
        parent->leftChild = left;
        parent->rightChild = right;
        heap.push_back(parent);
    }
    return parent;
}

void generateCodes(node *_node, std::string _binaryCode, std::map<char, std::string> &codes, std::string &dictionaryContent)
{
    if (!_node)
        return;

    if (_node->character != -1)
    {
        codes[_node->character] = _binaryCode;
        dictionaryContent += '1';
        dictionaryContent += _node->character;
    }
    else
        dictionaryContent += '0';

    generateCodes(_node->leftChild, _binaryCode + "0", codes, dictionaryContent);
    generateCodes(_node->rightChild, _binaryCode + "1", codes, dictionaryContent);
}

void readFile(std::string &inputFileName, std::vector<char> &text, std::map<char, unsigned int> &frequencies)
{
    char character; // a character to be read from input file
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open())
        throwError("Failed to open the input file.");

    while (inputFile.get(character))
    {
        text.push_back(character);
        frequencies[character]++;
    }

    if (!text.size())
        throwError("The input file is empty.");

    inputFile.close();
}

void readBinaryFile(std::string &inputFileName, std::vector<char> &text)
{
    char byte;
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open())
        throwError("Failed to open the input file.");

    while (inputFile.get(byte))
    {
        auto stringByte = std::bitset<8>(byte).to_string();

        for (int i = 0; i < stringByte.size(); i++)
            text.push_back(stringByte[i]);
    }

    if (!text.size())
        throwError("The input file is empty.");

    inputFile.close();
}

void printCompressedData(std::string &outputFileName, std::vector<char> &text, std::map<char, std::string> &codes)
{
    std::ofstream outputFile(outputFileName, std::ios::binary);
    std::string binaryCode = "";

    if (!outputFile.is_open())
        throwError("Failed to open the output file.");

    for (auto &&character : text)
    {
        binaryCode += codes[character];
        if (binaryCode.size() > 8)
        {
            auto x = std::bitset<8>(binaryCode.substr(0, 8)).to_ulong();
            outputFile.write((char *)&x, 1);
            binaryCode.erase(0, 8);
        }
    }
    if (binaryCode.size())
    {
        while (binaryCode.size() != 8)
            binaryCode += '0';
        auto x = std::bitset<8>(binaryCode.substr(0, 8)).to_ulong();
        outputFile.write((char *)&x, 1);
    }
    outputFile.close();
}

void printDecompressedData(std::string &outputFileName, std::vector<char> &text)
{
    std::ofstream outputFile(outputFileName);

    if (!outputFile.is_open())
        throwError("Failed to open the output file.");

    for (auto &&character : text)
        outputFile << character;

    outputFile.close();
}

void generateDictionaryFile(std::string &dictionaryFileName, std::string &dictionaryContent)
{
    std::ofstream dictionaryFile(dictionaryFileName);

    if (!dictionaryFile.is_open())
        throwError("Failed to open the dictionary file.");

    dictionaryFile << dictionaryContent;

    dictionaryFile.close();
}

void readDictionaryFile(std::string &dictionaryFileName, std::map<std::string, char> &codes)
{
    char character;             // a character to be read from dictionary file
    bool isAsciiSymbol = false; // a variable that indicates true when the next character is an ascii character and false if not
    std::string code = "";      // unique code of the character
    std::ifstream dictionaryFile(dictionaryFileName);

    if (!dictionaryFile.is_open())
        throwError("Failed to open the dictionary file.");

    while (dictionaryFile.get(character))
    {
        if (!isAsciiSymbol)
        {
            if (character == '0')
                code += '0';
            else if (character == '1')
                isAsciiSymbol = true;
            else
                throwError("Dictionary content is invalid.");
        }
        else
        {
            codes[code] = character;

            while (code.size() != 1 && code.back() == '1')
                code.erase(code.size() - 1);

            code.erase(code.size() - 1);
            code += '1';
            isAsciiSymbol = false;
        }
    }
    if (!codes.size())
        throwError("The dictionary file is empty.");
    for (auto &&i : codes)
        std::cout << i.first << " == " << i.second << std::endl;
}