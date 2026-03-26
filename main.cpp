#include <iostream>
#include <string>
#include <cstring>
#include "huffman.hpp"
#include "file_handler.hpp"

void printUsage(const char* programName) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "    Huffman File Compression Tool" << std::endl;
    std::cout << "========================================\n" << std::endl;
    std::cout << "Usage: " << programName << " <command> <input> <output>\n" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  -c, --compress    Compress a file" << std::endl;
    std::cout << "  -d, --decompress  Decompress a file" << std::endl;
    std::cout << "  -h, --help        Show this help message\n" << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " -c input.txt compressed.huff" << std::endl;
    std::cout << "  " << programName << " -d compressed.huff output.txt\n" << std::endl;
}

void printCodeTable(const huffman::CodeMap& codes) {
    std::cout << "\nHuffman Code Table:" << std::endl;
    std::cout << "-------------------" << std::endl;
    
    for (const auto& pair : codes) {
    char ch = pair.first;
    std::string code = pair.second;

    std::cout << "  '";
    if (ch == '\n') std::cout << "\\n";
    else if (ch == '\t') std::cout << "\\t";
    else if (ch == '\r') std::cout << "\\r";
    else if (ch == ' ') std::cout << "SP";
    else std::cout << ch;
    std::cout << "' : " << code << std::endl;
}
    std::cout << std::endl;
}

void interactiveMode() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "    Huffman File Compression Tool" << std::endl;
    std::cout << "        Interactive Mode" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    while (true) {
        std::cout << "Select an option:" << std::endl;
        std::cout << "  1. Compress a file" << std::endl;
        std::cout << "  2. Decompress a file" << std::endl;
        std::cout << "  3. Compress text (demo)" << std::endl;
        std::cout << "  4. Exit" << std::endl;
        std::cout << "\nChoice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            std::string input, output;
            std::cout << "Enter input file path: ";
            std::getline(std::cin, input);
            std::cout << "Enter output file path: ";
            std::getline(std::cin, output);
            
            huffman::FileHandler::compressFile(input, output);
        }
        else if (choice == 2) {
            std::string input, output;
            std::cout << "Enter compressed file path: ";
            std::getline(std::cin, input);
            std::cout << "Enter output file path: ";
            std::getline(std::cin, output);
            
            huffman::FileHandler::decompressFile(input, output);
        }
        else if (choice == 3) {
            std::cout << "Enter text to compress: ";
            std::string text;
            std::getline(std::cin, text);
            
            if (text.empty()) {
                std::cout << "Text cannot be empty!" << std::endl;
                continue;
            }
            
            huffman::HuffmanCoder coder;
            auto result = coder.compress(text);
            std::string treeData = result.first;
            std::string encodedData = result.second;
            
            std::cout << "\nOriginal text: \"" << text << "\"" << std::endl;
            std::cout << "Original size: " << text.size() * 8 << " bits" << std::endl;
            std::cout << "Encoded size: " << encodedData.size() << " bits" << std::endl;
            
            double ratio = (1.0 - static_cast<double>(encodedData.size()) / (text.size() * 8)) * 100;
            std::cout << "Compression ratio: " << ratio << "%" << std::endl;
            
            printCodeTable(coder.getCodes());
            
            // Verify decompression
            std::string decoded = coder.decompress(treeData, encodedData);
            std::cout << "Decoded text: \"" << decoded << "\"" << std::endl;
            std::cout << "Verification: " << (text == decoded ? "PASSED" : "FAILED") << std::endl;
        }
        else if (choice == 4) {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid choice!" << std::endl;
        }
        
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Interactive mode
        interactiveMode();
        return 0;
    }
    
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "-h" || command == "--help") {
        printUsage(argv[0]);
        return 0;
    }
    
    if (argc < 4) {
        std::cerr << "Error: Missing arguments" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    
    if (command == "-c" || command == "--compress") {
        if (!huffman::FileHandler::fileExists(inputFile)) {
            std::cerr << "Error: Input file does not exist: " << inputFile << std::endl;
            return 1;
        }
        
        if (huffman::FileHandler::compressFile(inputFile, outputFile)) {
            return 0;
        }
        return 1;
    }
    else if (command == "-d" || command == "--decompress") {
        if (!huffman::FileHandler::fileExists(inputFile)) {
            std::cerr << "Error: Input file does not exist: " << inputFile << std::endl;
            return 1;
        }
        
        if (huffman::FileHandler::decompressFile(inputFile, outputFile)) {
            return 0;
        }
        return 1;
    }
    else {
        std::cerr << "Error: Unknown command: " << command << std::endl;
        printUsage(argv[0]);
        return 1;
    }
}
