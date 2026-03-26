#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <memory>

namespace huffman {

// Node structure for Huffman Tree
struct HuffmanNode {
    char character;
    unsigned frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char ch, unsigned freq);
    HuffmanNode(unsigned freq, 
                std::shared_ptr<HuffmanNode> l, 
                std::shared_ptr<HuffmanNode> r);
    
    bool isLeaf() const;
};

// Comparator for priority queue (min-heap)
struct NodeComparator {
    bool operator()(const std::shared_ptr<HuffmanNode>& a,
                    const std::shared_ptr<HuffmanNode>& b) const;
};

// Type aliases
using NodePtr = std::shared_ptr<HuffmanNode>;
using FrequencyMap = std::unordered_map<char, unsigned>;
using CodeMap = std::unordered_map<char, std::string>;
using MinHeap = std::priority_queue<NodePtr, std::vector<NodePtr>, NodeComparator>;

class HuffmanCoder {
private:
    NodePtr root;
    CodeMap codes;
    FrequencyMap frequencies;

    void buildCodes(NodePtr node, const std::string& code);
    void serializeTree(NodePtr node, std::string& output) const;
    NodePtr deserializeTree(const std::string& data, size_t& index);

public:
    HuffmanCoder() = default;

    // Build frequency table from data
    FrequencyMap buildFrequencyTable(const std::string& data);

    // Build Huffman tree from frequencies
    NodePtr buildTree(const FrequencyMap& freqMap);

    // Generate codes from tree
    CodeMap generateCodes(NodePtr root);

    // Encode data using codes
    std::string encode(const std::string& data, const CodeMap& codes);

    // Decode binary string using tree
    std::string decode(const std::string& encodedData, NodePtr root);

    // Serialize tree to string for storage
    std::string serializeTree() const;

    // Deserialize tree from string
    void deserializeTree(const std::string& data);

    // Getters
    NodePtr getRoot() const { return root; }
    CodeMap getCodes() const { return codes; }
    FrequencyMap getFrequencies() const { return frequencies; }

    // Full compression/decompression
    std::pair<std::string, std::string> compress(const std::string& data);
    std::string decompress(const std::string& treeData, const std::string& encodedData);
};

} // namespace huffman

#endif // HUFFMAN_HPP
