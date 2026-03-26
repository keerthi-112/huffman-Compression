# Huffman File Compression Tool

## Overview

The Huffman File Compression Tool is a C++ console-based application that implements **lossless data compression** using Huffman Coding. It allows users to compress text files into a smaller binary format and later decompress them back to their original content without any loss.

The project demonstrates how real-world compression systems work by combining data structures, greedy algorithms, and file handling techniques.

---

## Key Features

* File Compression: Converts input text files into compressed binary format
* File Decompression: Restores compressed files back to original content
* Demo Mode: Allows users to test compression on custom text input
* Compression Statistics: Displays original size, compressed size, and compression ratio
* Bit-Level Processing: Efficient encoding and decoding using bit manipulation
* Interactive Menu: Easy-to-use CLI-based navigation

---

## Concepts Used

* Huffman Coding (Greedy Algorithm)
* Binary Tree (Huffman Tree)
* Priority Queue (Min Heap)
* File Handling in C++
* Bit Manipulation
* Encoding and Decoding Techniques

---

## Project Structure

* main.cpp → Handles user interaction and menu system
* huffman.* → Core logic for building tree, encoding, and decoding
* file_handler.* → Manages file compression and decompression
* bit_stream.* → Handles bit-level read and write operations

---

## How to Run

### Compile

```bash
g++ -std=c++17 *.cpp -o main
```

### Run

```bash id="xqz2hn"
.\main.exe
```

---

## Usage Guide

1. Compress a File

   * Enter input file name
   * Provide output compressed file name

2. Decompress a File

   * Enter compressed file
   * Provide output file name

3. Demo Mode

   * Enter custom text
   * View compression results and Huffman codes

4. Exit

   * Close the program

---

## Sample Output

Original Size: 152 bits
Compressed Size: 69 bits
Compression Ratio: ~55%

---

## Workflow of the System

1. Read input data (text or file)
2. Calculate frequency of each character
3. Build Huffman Tree using priority queue
4. Generate binary codes for each character
5. Encode data using generated codes
6. Store compressed data along with tree information
7. During decompression, rebuild tree and decode data
8. Output the original content

---

## Benefits of the Project

* Demonstrates real-world compression techniques used in ZIP, JPEG, and MP3
* Reduces storage requirements significantly
* Improves understanding of greedy algorithms and tree structures
* Shows practical use of file handling and bit-level operations

---

## Future Improvements

* Add command-line arguments (e.g., compress via single command)
* Support compression of different file types
* Add graphical user interface (GUI)
* Improve compression efficiency and benchmarking

---

## Author

Keerthi Mupparaju
