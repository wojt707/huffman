/** @file */

#ifndef LIBRARY_h
#define LIBRARY_h

/** @enum programMode
 * Defines what type of program is selected.
 */
enum programMode
{ /** program mode not set*/
  undefined,
  /** program mode set to compression*/
  compression,
  /** program mode set to decompression*/
  decompression
};

/** @struct node
 * Defines single node in Huffman Tree.
 */
struct node
{ /** character of the node if the node is not a parent node */
  char character;
  /** frequency of the node */
  unsigned int frequency;
  /** pointer to left child if the node is a parent node */
  node *leftChild;
  /** pointer to right child if the node is a parent node */
  node *rightChild;

  /** constructor of node */
  node(char _character, unsigned int _frequency)
  {
    leftChild = rightChild = NULL;
    character = _character;
    frequency = _frequency;
  }
};

/** The function handles program arguments
 *  @param argumentCounter number of program arguments
 *  @param arguments set of program arguments
 *  @param inputFileName a name of the input file
 *  @param outputFileName a name of the output file
 *  @param dictionaryFileName a name of the dictionary file
 *  @param programMode program mode (undefined/compression/decompression)
 */
void handleArguments(int argumentCounter,
                     char const *arguments[],
                     std::string &inputFileName,
                     std::string &outputFileName,
                     std::string &dictionaryFileName,
                     programMode &_programMode);

/** The function prints error to output and stops the program
 *  @param error a message to be printed
 */
void throwError(const std::string &error);

/** The function finds pointer to node with the lowest character frequency
 *  and deletes it from vector
 *  @param nodeSet a set of pointers to nodes
 *  @return a pointer to node with the lowest character frequency
 */
node *findMinimumNode(std::vector<node *> &nodeSet);

/** The function creates Huffman Tree
 *  @param frequencies a set of characters and their frequencies
 *  @return a pointer to root node
 */
node *createHuffmanTree(std::map<char, unsigned int> &frequencies);

/** The function generates binary code for each character, places it in codes map
 *  and creates specific dictionary content
 *  @param _node a pointer to node from the Huffman Tree
 *  @param _binaryCode a current binary code
 *  @param codes a set of characters and their binary codes
 *  @param dictionaryContent a special dictionary code that will later be printed to the dictionary file
 */
void generateCodes(node *_node, std::string _binaryCode, std::map<char, std::string> &codes, std::string &dictionaryContent);

/** The function reads the file character by character
 *  @param inputFileName a name of the input file
 *  @param text a vector in which all read characters are written
 *  @param frequencies a set of characters and their frequencies
 */
void readFile(std::string &inputFileName, std::vector<char> &text, std::map<char, unsigned int> &frequencies);

/** The function reads the binary file byte by byte
 *  @param inputFileName a name of the input file
 *  @param text a vector in which all read bits are written
 */
void readBinaryFile(std::string &inputFileName, std::vector<char> &text);

/** The function prints binary codes of characters to the output file
 *  @param outputFileName a name of the output file
 *  @param text a set of characters, the binary codes of which will be printed to the output file
 *  @param codes a set of characters and their binary codes
 */
void printCompressedData(std::string &outputFileName, std::vector<char> &text, std::map<char, std::string> &codes);

/** The function prints characters to the output file
 *  @param outputFileName a name of the output file
 *  @param text a set of characters to be printed
 */
void printDecompressedData(std::string &outputFileName, std::vector<char> &text);

/** The function prints special dictionary code to the dictionary file
 *  @param dictionaryFileName a name of the dictionary file
 *  @param dictionaryContent a dictionary code to be printed to the dictionary file
 */
void generateDictionaryFile(std::string &dictionaryFileName, std::string &dictionaryContent);

/** The function reads the dictionary file and places binary codes of characters to codes map
 *  @param dictionaryFileName a name of the dictionary file
 *  @param codes a set of binary codes and their characters
 */
void readDictionaryFile(std::string &dictionaryFileName, std::map<std::string, char> &codes);

#endif