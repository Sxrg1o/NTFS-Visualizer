#pragma once
#include "../imageReader/Reader.h"
#include "../../include/data_structures.h"
#include <type_traits>
#include <memory>
#include <iostream>

class StructReader {
public:
    // Reads a structure from the current reader
    template<typename T>
    static bool read(T& struct_obj, Reader* reader) {
        static_assert(std::is_standard_layout<T>::value, 
                     "Solo se pueden leer estructuras standard-layout");
        
        if (!reader) {
            std::cerr << "Reader inválido" << std::endl;
            return false;
        }

        auto bytesRead = reader->read(reinterpret_cast<uint8_t*>(&struct_obj), sizeof(T));
        
        if (bytesRead != sizeof(T)) {
            std::cerr << "Error al leer estructura. Bytes leídos: " 
                     << bytesRead << " de " << sizeof(T) << " esperados" << std::endl;
            return false;
        }

        return true;
    }

    // Prints the structure in hexadecimal format
    template<typename T>
    static bool readAndPrint(T& struct_obj, Reader* reader) {
        int64_t initialPos = reader->tell();
        
        if (!read(struct_obj, reader)) {
            return false;
        }

        std::cout << "Estructura leída en offset 0x" 
                 << std::hex << initialPos << std::dec << ":\n";
        
        HexPrinter::print(reinterpret_cast<uint8_t*>(&struct_obj), 
                         sizeof(T), 
                         initialPos);
        
        return true;
    }

    // Prints an array of bytes in hexadecimal format
    template <typename T, size_t N>
    static void printArrayHex(const T (&array)[N], const std::string& label) {
        std::cout << label << ": ";
        for (size_t i = 0; i < N; ++i) {
            std::cout << std::hex << std::uppercase 
                    << (int)array[i] << " ";
        }
        std::cout << std::dec << "\n";
    }
    
    // Testing method to print specific fields of bootSector
    static void printBootSectorInfo(const bootSector& bs) {
        std::cout << "\nInformación del Boot Sector:\n";
        printArrayHex(bs.jump_instruction, "Jump Instruction");
        std::cout << "OEM ID: " << bs.oem_name << "\n";
        std::cout << "Bytes por sector: " << bs.bytes_x_sector << "\n"; 
        std::cout << "Sectores por cluster: " << (int)bs.sectors_x_cluster << "\n";
        std::cout << "Sectores reservados: " << bs.reserved_sectors << "\n";
        std::cout << "Sectores por volumen: " << bs.sectors_x_volume << "\n";
        std::cout << "Cluster MFT inicio: " << bs.cluster_MFT_start << "\n";
        std::cout << "Tamaño de entrada: " << (int)bs.entry_size << "\n";
        std::cout << "Número serial: 0x" << std::hex << bs.serial_number << std::dec << "\n";
        std::cout << "Firma: 0x" << std::hex << bs.signature << std::dec << "\n";
    }

};