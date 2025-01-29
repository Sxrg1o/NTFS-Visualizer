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

};