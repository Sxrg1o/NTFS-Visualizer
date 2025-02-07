#include "../../include/structReader.h"
#include "../../include/read.h"
#include "../../include/data_structures.h"
#include "../../include/image.h"
#include "../../include/clusterProc.h"
#include "../../include/HexPrinter.h"

ntfsImage image = {};
bootSector bs = {};

bool read_boot_sector(const std::unique_ptr<Reader>& reader) {
    reader->seek(0);    // Boot sector always at offset 0
    if (!StructReader::read(bs, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return false;
    }
    image.bytes_x_sector = bs.bytes_x_sector;
    image.sectors_x_cluster = bs.sectors_x_cluster;
    image.cluster_MFT_start = bs.cluster_MFT_start;
    image.entry_size = bs.entry_size;
    image.index_size = bs.index_size;

    return true;
}

py::dict boot_sector_dict() {
    py::dict result;
    bool isValid = read_boot_sector(global_reader);
    if(!isValid) {
        return result;
    }
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

std::string boot_sector_hex() {
    std::stringstream ss;
    HexPrinter::print(bs, ss);
    return ss.str();
}


