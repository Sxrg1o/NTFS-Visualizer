#include "ReaderFactory.h"
#include "../ntfs/structReader.h"
#include <iostream>
#include <iomanip>

int main() {
    auto reader = ReaderFactory::createReader(false);
    if (!reader->open("/home/sergio/Desktop/Filesystem-Visualizer/tests/imagen.img")) {
        std::cerr << "Error opening file/partition" << std::endl;
        return 1;
    }

    bootSector bs = {};
    if (StructReader::readAndPrint(bs, reader.get())) {
        StructReader::printBootSectorInfo(bs);
    } else {
        std::cerr << "Error reading boot sector" << std::endl;
    }

    reader->close();
}