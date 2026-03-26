#ifndef BIT_STREAM_HPP
#define BIT_STREAM_HPP

#include <vector>
#include <string>
#include <cstdint>

namespace huffman {

class BitWriter {
private:
    std::vector<uint8_t> buffer;
    uint8_t currentByte;
    int bitPosition;

public:
    BitWriter();
    
    void writeBit(bool bit);
    void writeBits(const std::string& bits);
    void writeByte(uint8_t byte);
    void writeBytes(const std::string& data);
    void writeUint32(uint32_t value);
    
    void flush();
    std::vector<uint8_t> getData() const;
    size_t size() const;
};

class BitReader {
private:
    const std::vector<uint8_t>& buffer;
    size_t byteIndex;
    int bitPosition;

public:
    BitReader(const std::vector<uint8_t>& data);
    
    bool readBit();
    std::string readBits(size_t count);
    uint8_t readByte();
    std::string readBytes(size_t count);
    uint32_t readUint32();
    
    bool hasMore() const;
    size_t remaining() const;
};

} // namespace huffman

#endif // BIT_STREAM_HPP
