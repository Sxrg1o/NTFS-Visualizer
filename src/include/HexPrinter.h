#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

class HexPrinter {
public:

    static void print(const std::vector<uint8_t>& data, std::ostream& out = std::cout) {
        const size_t size = data.size();
        const uint8_t* buffer = data.data();

        const size_t BYTES_PER_LINE = 16;        
        for (size_t i = 0; i < size; i += BYTES_PER_LINE) {
            out << std::setfill('0') << std::setw(8) << std::hex << i << " ";
            
            for (size_t j = 0; j < BYTES_PER_LINE; j++) {
                out << std::setfill('0') << std::setw(2) << std::hex
                    << static_cast<int>(buffer[i + j]) << ' ';
            }
            
            out << " |";
            for (size_t j = 0; j < BYTES_PER_LINE && (i + j) < size; j++) {
                char c = static_cast<char>(buffer[i + j]);
                out << (std::isprint(c) ? c : '.');
            }
            out << "|\n";
        }
        out << std::dec;
    }

    template<typename T>
    static void print(const T& structure, std::ostream& out = std::cout) {
        const uint8_t* buffer = reinterpret_cast<const uint8_t*>(&structure);
        const size_t size = sizeof(T);
        
        const size_t BYTES_PER_LINE = 16;
        for (size_t i = 0; i < size; i += BYTES_PER_LINE) {
            out << std::setfill('0') << std::setw(8) << std::hex << i << " ";
            
            for (size_t j = 0; j < BYTES_PER_LINE; j++) {
                out << std::setfill('0') << std::setw(2) << std::hex
                    << static_cast<int>(buffer[i + j]) << ' '; 
            }
            
            out << " |";
            for (size_t j = 0; j < BYTES_PER_LINE && (i + j) < size; j++) {
                char c = static_cast<char>(buffer[i + j]);
                out << (std::isprint(c) ? c : '.');
            }
            out << "|\n";
        }
        out << std::dec;
    }
};