#include "../../include/imageFunc.h"
#include "ReaderFactory.h"
#include "../../include/image.h"
#include "../../include/data_structures.h"
#include "../ntfs/clusterProc.cpp"
#include <iostream>
#include <iomanip>
#include <cstring>

std::unique_ptr<Reader> global_reader;

py::dict readImage(const std::string& filePath) {
    global_reader = ReaderFactory::createReader(false);
    if (!global_reader->open(filePath)) {
        std::cerr << "Error opening file/partition" << std::endl;
        return py::dict();
    }

    bootSector bs = readBootSector(global_reader);
    if(bs.oem_name != FS_MAGIC) {
        std::cerr << "Not an NTFS image" << std::endl;
        global_reader->close();
        return py::dict();
    } else {
        std::cout << "Found NTFS image" << std::endl;
    }

    py::dict result;
    
    // Convert jump instruction to hex string
    std::stringstream ss;
    ss << std::hex << std::uppercase;
    for(int i = 0; i < 3; i++) {
        ss << std::setw(2) << std::setfill('0') << static_cast<int>(bs.jump_instruction[i]) << " ";
    }
    
    result["jump_instruction"] = ss.str();
    result["oem_name"] = std::string(reinterpret_cast<char*>(&bs.oem_name), 8);
    result["bytes_x_sector"] = bs.bytes_x_sector;
    result["sectors_x_cluster"] = static_cast<int>(bs.sectors_x_cluster);
    result["reserved_sectors"] = bs.reserved_sectors;
    result["media_descriptor"] = static_cast<std::string>(std::to_string(bs.media_descriptor));
    result["sectors_x_volume"] = bs.sectors_x_volume;
    result["cluster_MFT_start"] = bs.cluster_MFT_start;
    result["cluster_MFTMirr_start"] = bs.cluster_MFTMirr_start;
    result["entry_size"] = static_cast<int>(bs.entry_size);
    result["index_size"] = static_cast<int>(bs.index_size);
    result["serial_number"] = bs.serial_number;

    return result;
}

void closeImage() {
    if (global_reader) {
        global_reader->close();
    }
}