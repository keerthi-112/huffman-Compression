#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace huffman {

// File format:
// [4 bytes] Magic number "HUFF"
// [4 bytes] Original file size
// [4 bytes] Tree data size
// [4 bytes] Padding bits count
// [N bytes] Serialized tree
// [M bytes] Compressed data

struct CompressedFileHeader {
    char magic[4] = {'H', 'U', 'F', 'F'};
    uint32_t originalSize;
    uint32_t treeSize;
    uint32_t paddingBits;
};

class FileHandler {
public:
    // Read entire file as string
    static std::string readFile(const std::string& filename);
    
    // Read file as binary
    static std::vector<uint8_t> readBinaryFile(const std::string& filename);
    
    // Write string to file
    static void writeFile(const std::string& filename, const std::string& content);
    
    // Write binary data to file
    static void writeBinaryFile(const std::string& filename, 
                                const std::vector<uint8_t>& data);
    
    // Compress file
    static bool compressFile(const std::string& inputFile, 
                             const std::string& outputFile);
    
    // Decompress file
    static bool decompressFile(const std::string& inputFile, 
                               const std::string& outputFile);
    
    // Get file size
    static size_t getFileSize(const std::string& filename);
    
    // Check if file exists
    static bool fileExists(const std::string& filename);
};

} // namespace huffman

#endif // FILE_HANDLER_HPP
