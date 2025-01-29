#include "../ntfs/structReader.h"
#include "../imageReader/ReaderFactory.h"
#include "../../include/data_structures.h"

bootSector readBootSector(const std::unique_ptr<Reader>& reader) {
    bootSector bs = {};
    if (!StructReader::read(bs, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
    }
    return bs;
}

