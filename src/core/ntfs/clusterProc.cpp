#include "../../include/structReader.h"
#include "../../include/read.h"
#include "../../include/data_structures.h"
#include "../../include/image.h"
#include "../../include/clusterProc.h"
#include "../../include/HexPrinter.h"
#include "../../include/entryProc.h"
#include "../../include/attrProc.h"
#include <vector>

ntfsImage image = {};
bootSector bs = {};

/*****  Reading the boot sector   *****/

bool read_boot_sector(const std::unique_ptr<Reader>& reader) {
    reader->seek(0, false);    // Boot sector always at offset 0
    if (!StructReader::read(bs, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return false;
    }
    image.bytes_x_sector = bs.bytes_x_sector;
    image.sectors_x_cluster = bs.sectors_x_cluster;
    image.cluster_MFT_start = bs.cluster_MFT_start;
    image.entry_size = bs.entry_size;
    image.index_size = bs.index_size;
    image.sectors_x_volume = bs.sectors_x_volume;

    return true;
}

/*py::dict boot_sector_dict() {
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
}*/

std::string boot_sector_hex() {
    std::stringstream ss;
    HexPrinter::print(bs, ss);
    return ss.str();
}

/*****  Reading cluster info  *****/

ClusterStatus analyze_clusters(uint64_t chunk) {
    ClusterStatus status;
    uint64_t total_clusters = image.sectors_x_volume / image.sectors_x_cluster;
    uint64_t max_chunks = (total_clusters + CLUSTERS_X_CHUNK - 1) / CLUSTERS_X_CHUNK;

    if (chunk >= max_chunks) {
        chunk = max_chunks - 1;
    }

    uint64_t start_cluster = chunk * CLUSTERS_X_CHUNK;
    uint64_t end_cluster = std::min(start_cluster + CLUSTERS_X_CHUNK, total_clusters);
    uint64_t clusters_this_chunk = end_cluster - start_cluster;
    
    // Read $Bitmap structure
    mftEntry bitmap_entry = read_mft_entry(global_reader, MDF_BITMAP);
    mftAttr *data_attr = find_attribute(bitmap_entry, ATTR_DATA, 0);
    dataAttr bitmap_data = read_data_attribute(global_reader, data_attr, MDF_BITMAP);
    
    uint64_t start_byte = start_cluster / 8;
    uint64_t end_byte = (end_cluster + 7) / 8;  
    uint64_t bytes_needed = end_byte - start_byte;
    
    // Now we read $DATA from the bitmap
    std::vector<uint8_t> bitmap_portion = read_data_portion(global_reader, bitmap_data, chunk, clusters_this_chunk, MDF_BITMAP);

    if (bitmap_portion.empty()) {
        status.clusters.resize(clusters_this_chunk, ClusterStatus::FREE);
        return status;
    }
    status.clusters.resize(clusters_this_chunk);
    
    for (uint64_t i = 0; i < clusters_this_chunk; i++) {
        uint64_t current_cluster = start_cluster + i;
        uint64_t byte_pos = (current_cluster / 8) - start_byte; 
        uint8_t bit_pos = current_cluster % 8;
        
        bool is_allocated = (bitmap_portion[byte_pos] & (1 << bit_pos)) != 0;
        status.clusters[i] = is_allocated ? ClusterStatus::USED : ClusterStatus::FREE;
    }

    // Read $BadClus structure
    mftEntry bad_clus_entry = read_mft_entry(global_reader, MDF_BADCL);
    mftAttr *bad_clus_attr = find_attribute(bad_clus_entry, ATTR_DATA, 2);
    dataAttr bad_clus_data = read_data_attribute(global_reader, bad_clus_attr, MDF_BADCL);

    uint64_t absolute_cluster = 0;
    uint64_t physical_cluster = 0;

    for (const auto& run : bad_clus_data.runs) {
        if (run.cluster_count == 0) continue;       // Ignore sparse runs
        
        if (run.cluster_offset != 0) {      
            physical_cluster += run.cluster_offset;
            uint64_t run_start = physical_cluster;
            uint64_t run_end = run_start + run.cluster_count;

            if (run_start < end_cluster && run_end > start_cluster) {
                uint64_t intersection_start = std::max(run_start, start_cluster);
                uint64_t intersection_end = std::min(run_end, end_cluster);

                for (uint64_t cluster = intersection_start; cluster < intersection_end; cluster++) {
                    uint64_t index = cluster - start_cluster;
                    if (index < status.clusters.size()) {
                        status.clusters[index] = ClusterStatus::BAD;
                    }
                }
            }
        }
        
        absolute_cluster += run.cluster_count;
    }

    return status;
}