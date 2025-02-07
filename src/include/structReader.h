#pragma once
#include "read.h"
#include "data_structures.h"
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

};