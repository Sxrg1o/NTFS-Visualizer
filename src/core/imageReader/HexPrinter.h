#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

class HexPrinter {
public:
    // Prints a buffer in hexadecimal format with offset, bytes in hex and ASCII
    static void print(const uint8_t* buffer, size_t size, int64_t offset = 0) {
        const size_t BYTES_PER_LINE = 16;
        
        for (size_t i = 0; i < size; i += BYTES_PER_LINE) {
            std::cout << std::setfill('0') << std::setw(8) << std::hex << (offset + i) << "  ";
            
            for (size_t j = 0; j < BYTES_PER_LINE; j++) {
                if (j == 8) {
                    std::cout << ' '; 
                }
                
                if (i + j < size) {
                    std::cout << std::setfill('0') << std::setw(2) << std::hex 
                        << static_cast<int>(buffer[i + j]) << ' ';
                } else {
                    std::cout << "   "; 
                }
            }
            
            std::cout << " |";
            for (size_t j = 0; j < BYTES_PER_LINE && (i + j) < size; j++) {
                char c = static_cast<char>(buffer[i + j]);
                std::cout << (std::isprint(c) ? c : '.');
            }
            std::cout << "|\n";
        }
        
        std::cout << std::dec;
    }
    
    // Prints only a certain amount of lines
    static void printLines(const uint8_t* buffer, size_t size, size_t numLines, int64_t offset = 0) {
        const size_t BYTES_PER_LINE = 16;
        size_t bytesToPrint = std::min(size, numLines * BYTES_PER_LINE);
        print(buffer, bytesToPrint, offset);
        
        if (size > bytesToPrint) {
            std::cout << "...\n";
        }
    }
};