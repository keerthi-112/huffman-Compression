#include "huffman.hpp"
#include <stdexcept>

namespace huffman {

// HuffmanNode constructors
HuffmanNode::HuffmanNode(char ch, unsigned freq)
    : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

HuffmanNode::HuffmanNode(unsigned freq, NodePtr l, NodePtr r)
    : character('\0'), frequency(freq), left(l), right(r) {}

bool HuffmanNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

// NodeComparator
bool NodeComparator::operator()(const NodePtr& a, const NodePtr& b) const {
    return a->frequency > b->frequency;
}

// Build frequency table
FrequencyMap HuffmanCoder::buildFrequencyTable(const std::string& data) {
    frequencies.clear();
    for (char ch : data) {
        frequencies[ch]++;
    }
    return frequencies;
}

// Build Huffman tree
NodePtr HuffmanCoder::buildTree(const FrequencyMap& freqMap) {
    if (freqMap.empty()) {
        return nullptr;
    }

    MinHeap minHeap;

    // Create leaf nodes and add to heap
    for (const auto& pair : freqMap) {
        minHeap.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }

    // Handle single character case
    if (minHeap.size() == 1) {
        NodePtr single = minHeap.top();
        minHeap.pop();
        root = std::make_shared<HuffmanNode>(single->frequency, single, nullptr);
        return root;
    }

    // Build tree by combining nodes
    while (minHeap.size() > 1) {
        NodePtr left = minHeap.top();
        minHeap.pop();

        NodePtr right = minHeap.top();
        minHeap.pop();

        unsigned combinedFreq = left->frequency + right->frequency;
        NodePtr parent = std::make_shared<HuffmanNode>(combinedFreq, left, right);
        
        minHeap.push(parent);
    }

    root = minHeap.top();
    return root;
}

// Generate codes recursively
void HuffmanCoder::buildCodes(NodePtr node, const std::string& code) {
    if (!node) return;

    if (node->isLeaf()) {
        codes[node->character] = code.empty() ? "0" : code;
        return;
    }

    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

CodeMap HuffmanCoder::generateCodes(NodePtr treeRoot) {
    codes.clear();
    buildCodes(treeRoot, "");
    return codes;
}

// Encode data
std::string HuffmanCoder::encode(const std::string& data, const CodeMap& codeMap) {
    std::string encoded;
    encoded.reserve(data.size() * 8); // Pre-allocate

    for (char ch : data) {
        auto it = codeMap.find(ch);
        if (it == codeMap.end()) {
            throw std::runtime_error("Character not found in code map: " + std::string(1, ch));
        }
        encoded += it->second;
    }

    return encoded;
}

// Decode data
std::string HuffmanCoder::decode(const std::string& encodedData, NodePtr treeRoot) {
    if (!treeRoot) {
        throw std::runtime_error("Huffman tree is empty");
    }

    std::string decoded;
    NodePtr current = treeRoot;

    for (char bit : encodedData) {
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        } else {
            throw std::runtime_error("Invalid bit in encoded data");
        }

        if (!current) {
            throw std::runtime_error("Invalid encoded data - null node reached");
        }

        if (current->isLeaf()) {
            decoded += current->character;
            current = treeRoot;
        }
    }

    return decoded;
}

// Serialize tree (preorder traversal)
void HuffmanCoder::serializeTree(NodePtr node, std::string& output) const {
    if (!node) return;

    if (node->isLeaf()) {
        output += '1';
        output += node->character;
    } else {
        output += '0';
        serializeTree(node->left, output);
        serializeTree(node->right, output);
    }
}

std::string HuffmanCoder::serializeTree() const {
    std::string serialized;
    serializeTree(root, serialized);
    return serialized;
}

// Deserialize tree
NodePtr HuffmanCoder::deserializeTree(const std::string& data, size_t& index) {
    if (index >= data.size()) {
        return nullptr;
    }

    if (data[index] == '1') {
        index++;
        if (index >= data.size()) {
            throw std::runtime_error("Invalid tree data");
        }
        char ch = data[index++];
        return std::make_shared<HuffmanNode>(ch, 0);
    } else {
        index++;
        NodePtr left = deserializeTree(data, index);
        NodePtr right = deserializeTree(data, index);
        return std::make_shared<HuffmanNode>(0, left, right);
    }
}

void HuffmanCoder::deserializeTree(const std::string& data) {
    size_t index = 0;
    root = deserializeTree(data, index);
}

// Full compression
std::pair<std::string, std::string> HuffmanCoder::compress(const std::string& data) {
    if (data.empty()) {
        return {"", ""};
    }

    buildFrequencyTable(data);
    buildTree(frequencies);
    generateCodes(root);

    std::string treeData = serializeTree();
    std::string encodedData = encode(data, codes);

    return {treeData, encodedData};
}

// Full decompression
std::string HuffmanCoder::decompress(const std::string& treeData, const std::string& encodedData) {
    if (treeData.empty() || encodedData.empty()) {
        return "";
    }

    deserializeTree(treeData);
    return decode(encodedData, root);
}

} // namespace huffman
