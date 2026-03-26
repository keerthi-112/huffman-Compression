#include "file_handler.hpp"
#include "huffman.hpp"
#include "bit_stream.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace huffman {

std::string FileHandler::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<uint8_t> FileHandler::readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Error reading file: " + filename);
    }
    
    return buffer;
}

void FileHandler::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    file.write(content.data(), content.size());
}

void FileHandler::writeBinaryFile(const std::string& filename, 
                                   const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

bool FileHandler::compressFile(const std::string& inputFile, 
                                const std::string& outputFile) {
    try {
        // Read input file
        std::string content = readFile(inputFile);
        if (content.empty()) {
            std::cerr << "Input file is empty" << std::endl;
            return false;
        }
        
        // Compress using Huffman coding
        HuffmanCoder coder;
        auto result = coder.compress(content);
        std::string treeData = result.first;
        std::string encodedData = result.second;
        
        // Calculate padding
        uint32_t paddingBits = (8 - (encodedData.size() % 8)) % 8;
        
        // Create header
        CompressedFileHeader header;
        header.originalSize = static_cast<uint32_t>(content.size());
        header.treeSize = static_cast<uint32_t>(treeData.size());
        header.paddingBits = paddingBits;
        
        // Write to output file
        BitWriter writer;
        
        // Write header
        for (int i = 0; i < 4; i++) {
            writer.writeByte(header.magic[i]);
        }
        writer.writeUint32(header.originalSize);
        writer.writeUint32(header.treeSize);
        writer.writeUint32(header.paddingBits);
        
        // Write tree data
        writer.writeBytes(treeData);
        
        // Write encoded data
        writer.writeBits(encodedData);
        
        // Add padding
        for (uint32_t i = 0; i < paddingBits; i++) {
            writer.writeBit(false);
        }
        
        writer.flush();
        writeBinaryFile(outputFile, writer.getData());
        
        // Print statistics
        size_t originalSize = content.size();
        size_t compressedSize = writer.getData().size();
        double ratio = (1.0 - static_cast<double>(compressedSize) / originalSize) * 100;
        
        std::cout << "Compression successful!" << std::endl;
        std::cout << "Original size: " << originalSize << " bytes" << std::endl;
        std::cout << "Compressed size: " << compressedSize << " bytes" << std::endl;
        std::cout << "Compression ratio: " << ratio << "%" << std::endl;
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Compression error: " << e.what() << std::endl;
        return false;
    }
}

bool FileHandler::decompressFile(const std::string& inputFile, 
                                  const std::string& outputFile) {
    try {
        // Read compressed file
        std::vector<uint8_t> data = readBinaryFile(inputFile);
        if (data.size() < sizeof(CompressedFileHeader)) {
            throw std::runtime_error("Invalid compressed file");
        }
        
        BitReader reader(data);
        
        // Read and verify header
        char magic[4];
        for (int i = 0; i < 4; i++) {
            magic[i] = static_cast<char>(reader.readByte());
        }
        
        if (std::memcmp(magic, "HUFF", 4) != 0) {
            throw std::runtime_error("Invalid file format - not a HUFF file");
        }
        
        uint32_t originalSize = reader.readUint32();
        uint32_t treeSize = reader.readUint32();
        uint32_t paddingBits = reader.readUint32();
        
        // Read tree data
        std::string treeData = reader.readBytes(treeSize);
        
        // Read encoded data
        size_t encodedBits = reader.remaining() - paddingBits;
        std::string encodedData = reader.readBits(encodedBits);
        
        // Decompress
        HuffmanCoder coder;
        std::string decompressed = coder.decompress(treeData, encodedData);
        
        // Verify size
        if (decompressed.size() != originalSize) {
            std::cerr << "Warning: Decompressed size mismatch" << std::endl;
        }
        
        // Write output
        writeFile(outputFile, decompressed);
        
        std::cout << "Decompression successful!" << std::endl;
        std::cout << "Output size: " << decompressed.size() << " bytes" << std::endl;
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Decompression error: " << e.what() << std::endl;
        return false;
    }
}

size_t FileHandler::getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        return 0;
    }
    return static_cast<size_t>(file.tellg());
}

bool FileHandler::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

} // namespace huffman
