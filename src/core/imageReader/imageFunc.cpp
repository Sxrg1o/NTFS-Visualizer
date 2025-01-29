#include "ReaderFactory.h"
#include "../../include/image.h"
#include "../../include/data_structures.h"
#include "../ntfs/clusterProc.cpp"
#include <iostream>
#include <iomanip>
#include <cstring>

int readImage(const std::string& filePath) {
    auto reader = ReaderFactory::createReader(false);
    if (!reader->open(filePath)) {
        std::cerr << "Error opening file/partition" << std::endl;
        return -1;
    }

    // Read boot sector and check if it is ntfs image

    bootSector bs = readBootSector(reader);
    if(bs.oem_name != FS_MAGIC) {
        std::cerr << "Not an NTFS image" << std::endl;
        return -1;
    }

    // Asign values to ntfs structure
    ntfsImage im = {};
    im.bytes_x_sector = bs.bytes_x_sector;
    im.cluster_MFT_start = bs.cluster_MFT_start;
    im.entry_size = bs.entry_size;
    im.index_size = bs.index_size;
    im.sectors_x_cluster = bs.sectors_x_cluster;

    reader->close();

    return 1;
}

int main() {
    readImage("/home/sergio/Desktop/Filesystem-Visualizer/tests/imagen.img");

    return 0;
}