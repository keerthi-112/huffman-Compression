#include "bit_stream.hpp"
#include <stdexcept>

namespace huffman {

// BitWriter implementation
BitWriter::BitWriter() : currentByte(0), bitPosition(0) {}

void BitWriter::writeBit(bool bit) {
    if (bit) {
        currentByte |= (1 << (7 - bitPosition));
    }
    bitPosition++;
    
    if (bitPosition == 8) {
        buffer.push_back(currentByte);
        currentByte = 0;
        bitPosition = 0;
    }
}

void BitWriter::writeBits(const std::string& bits) {
    for (char c : bits) {
        writeBit(c == '1');
    }
}

void BitWriter::writeByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        writeBit((byte >> i) & 1);
    }
}

void BitWriter::writeBytes(const std::string& data) {
    for (char c : data) {
        writeByte(static_cast<uint8_t>(c));
    }
}

void BitWriter::writeUint32(uint32_t value) {
    writeByte((value >> 24) & 0xFF);
    writeByte((value >> 16) & 0xFF);
    writeByte((value >> 8) & 0xFF);
    writeByte(value & 0xFF);
}

void BitWriter::flush() {
    if (bitPosition > 0) {
        buffer.push_back(currentByte);
        currentByte = 0;
        bitPosition = 0;
    }
}

std::vector<uint8_t> BitWriter::getData() const {
    std::vector<uint8_t> result = buffer;
    if (bitPosition > 0) {
        result.push_back(currentByte);
    }
    return result;
}

size_t BitWriter::size() const {
    return buffer.size() * 8 + bitPosition;
}

// BitReader implementation
BitReader::BitReader(const std::vector<uint8_t>& data) 
    : buffer(data), byteIndex(0), bitPosition(0) {}

bool BitReader::readBit() {
    if (byteIndex >= buffer.size()) {
        throw std::runtime_error("Attempt to read past end of buffer");
    }
    
    bool bit = (buffer[byteIndex] >> (7 - bitPosition)) & 1;
    bitPosition++;
    
    if (bitPosition == 8) {
        byteIndex++;
        bitPosition = 0;
    }
    
    return bit;
}

std::string BitReader::readBits(size_t count) {
    std::string result;
    result.reserve(count);
    
    for (size_t i = 0; i < count; i++) {
        result += readBit() ? '1' : '0';
    }
    
    return result;
}

uint8_t BitReader::readByte() {
    uint8_t byte = 0;
    for (int i = 7; i >= 0; i--) {
        if (readBit()) {
            byte |= (1 << i);
        }
    }
    return byte;
}

std::string BitReader::readBytes(size_t count) {
    std::string result;
    result.reserve(count);
    
    for (size_t i = 0; i < count; i++) {
        result += static_cast<char>(readByte());
    }
    
    return result;
}

uint32_t BitReader::readUint32() {
    uint32_t value = 0;
    value |= static_cast<uint32_t>(readByte()) << 24;
    value |= static_cast<uint32_t>(readByte()) << 16;
    value |= static_cast<uint32_t>(readByte()) << 8;
    value |= static_cast<uint32_t>(readByte());
    return value;
}

bool BitReader::hasMore() const {
    return byteIndex < buffer.size();
}

size_t BitReader::remaining() const {
    if (byteIndex >= buffer.size()) return 0;
    return (buffer.size() - byteIndex) * 8 - bitPosition;
}

} // namespace huffman
