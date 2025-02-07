#include "../../include/imageFunc.h"
#include "../../include/read.h"
#include "../../include/image.h"
#include "../../include/data_structures.h"
#include <iostream>
#include <iomanip>
#include <cstring>

std::unique_ptr<Reader> global_reader;

bool is_ntfs() {
    if (!global_reader) {
        std::cerr << "No se ha abierto un archivo/partición" << std::endl;
        return false;
    }

    global_reader->seek(3);
    uint64_t oem_name;
    global_reader->read(reinterpret_cast<uint8_t*>(&oem_name), 8);
    global_reader->seek(0); // Reset

    if(oem_name != FS_MAGIC) {
        global_reader->close();
        return false;
    } else {
        return true;
    }
}

bool read_image(const std::string& filePath) {
    global_reader = ReaderFactory::createReader(false);
    if (!global_reader->open(filePath)) {
        std::cerr << "Error opening file/partition" << std::endl;
        return false;
    }

    if(!is_ntfs()) {
        return false;
    }

    return true;
}

void close_image() {
    if (global_reader) {
        global_reader->close();
    }
}