#include <iostream>
#include <iomanip>
#include <memory>
#include "../include/data_structures.h"
#include "../include/read.h"
#include "../include/image.h"
#include "../include/entryProc.h"
#include "../include/attrProc.h"
#include "../include/HexPrinter.h"
#include "../include/imageFunc.h"
#include "../include/clusterProc.h"

/** Comentar líneas referentes a pybind para compilar **/

void print_data_attribute(uint64_t entry_number, uint64_t attribute_type) {
    if (!global_reader) {
        std::cerr << "No file/partition opened" << std::endl;
        return;
    }

    /*mftEntry entry = read_mft_entry(global_reader, entry_number);
    mftAttr* data_attr = find_attribute(entry, ATTR_STDINF, 0);  // Just for testing
    if (!data_attr) {
        std::cerr << "No attribute found in entry " << entry_number << std::endl;
        return;
    }

    standardInfo info = get_stdinf_data(data_attr, entry_number);
    auto format_filetime = [](uint64_t filetime) {
        // FILETIME es el número de 100-nanosegundos intervalos desde Enero 1, 1601
        // Convertir a time_t (segundos desde epoch Unix)
        const uint64_t WINDOWS_TICK = 10000000;
        const uint64_t SEC_TO_UNIX_EPOCH = 11644473600LL;
        time_t timestamp = (filetime / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
        
        char buffer[30];
        struct tm* timeinfo = localtime(&timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return std::string(buffer);
    };

    std::cout << "\n$STANDARD_INFORMATION Attribute Values:" << std::endl;
    std::cout << "Created: " << format_filetime(info.creation_time) << std::endl;
    std::cout << "Modified: " << format_filetime(info.modified_time) << std::endl;
    std::cout << "MFT Modified: " << format_filetime(info.mft_modified_time) << std::endl;
    std::cout << "Accessed: " << format_filetime(info.accessed_time) << std::endl;
    std::cout << "DOS Permissions: 0x" << std::hex << info.dos_perms << std::dec << std::endl;
    std::cout << "Max Version: " << info.max_version << std::endl;
    std::cout << "Version: " << info.version << std::endl;
    std::cout << "Class ID: " << info.class_id << std::endl;*/

    /*dataAttr data = read_data_attribute(global_reader, data_attr, entry_number);

    std::cout << "Entry number: " << entry_number << std::endl;
    std::cout << "Attribute type: $DATA" << std::endl;      // Just for testing
    std::cout << "Resident: " << (data.is_resident ? "Yes" : "No") << std::endl;
    std::cout << "Logical size: " << data.logical_size << " bytes" << std::endl;

    std::vector<uint8_t> data_portion = read_data_portion(global_reader, data, 0, 200, entry_number);
    std::cout << "\nData content (hex):\n";
    for (size_t i = 0; i < data_portion.size(); ++i) {
        if (i > 0 && i % 16 == 0) std::cout << std::endl;
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(data_portion[i]) << " ";
    }
    std::cout << std::dec << std::endl;
    std::cout << "\nData content (ascii):\n";
    for (size_t i = 0; i < data_portion.size(); ++i) {
        if (i > 0 && i % 16 == 0) std::cout << std::endl;
        std::cout << static_cast<char>(data_portion[i]);
    }
    std::cout << std::dec << std::endl;

    if (!data.is_resident && !data.runs.empty()) {
        std::cout << "\nRun list information:\n";
        for (const auto& run : data.runs) {
            std::cout << "VCN Start/End: " << data_attr->content.non_resident_attr.VCN_start << "/" << data_attr->content.non_resident_attr.VCN_end << std::endl;
            std::cout << "Non-resident data offset: " << data_attr->content.non_resident_attr.offset << std::endl;
            std::cout << " Cluster count: " << run.cluster_count 
                      << ", Offset: " << run.cluster_offset << std::endl;
            uint64_t start_cluster = run.cluster_offset / image.bytes_x_sector / image.sectors_x_cluster;
            std::cout << " Clusters: ";
            for(int i = 0; i < run.cluster_count; i++) {
                std::cout << start_cluster << " ";
                start_cluster++;
            }
            std::cout << ", Sparse: " << (run.is_sparse ? "Yes" : "No") << std::endl;
        }
    }*/

    get_attr_def_info();
    get_volume_info();

    std::vector<mftEntry> list = read_entries(global_reader, 0);
    for(mftEntry entry : list) {
        mftAttr* stdinf_attr = find_attribute(entry, ATTR_STDINF, 0);
        standardInfo stdinf = get_stdinf_data(stdinf_attr, entry.number);
        mftAttr* data_attr = find_attribute(entry, ATTR_DATA, 0);
        uint64_t data_size;
        if(data_attr == nullptr) {
            data_size = 0;
        } else if(data_attr->header.resident_flag == ATTR_RESIDENT) {
            data_size = data_attr->content.resident_attr.size;
        } else {
            data_size = data_attr->content.non_resident_attr.actual_size;
        }
        
        std::cout << std::left << std::setw(5) << entry.number << std::setw(20) << get_file_name(entry) << std::setw(10) << data_size << std::right << get_file_permissions_string(stdinf.dos_perms) << std::endl;
        for(auto &attr : entry.attrs) {
            std::cout << "Attribute type: " << get_attr_name(attr.header.type) << (attr.header.resident_flag ? " (Non-resident)" : " (Resident)") << std::endl; 
        }        
        std::cout << std::endl;
    }

    /*ClusterStatus status = analyze_clusters(0);
    std::cout << "Cluster status:\n";
    int i = 0;
    for(auto &cluster : status.clusters) {
        if(i % 16 == 0 && i != 0) std::cout << std::endl;
        std::cout << cluster << " ";
        i++;
    }
    std::cout << std::endl;*/

    //std::string cluster_raw = get_cluster_raw(global_reader, 0);
    //std::cout << "Raw cluster data:\n";
    //std::cout << cluster_raw << std::endl;

}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <image_path> <entry_number> <attribute_type>" << std::endl;
        return 1;
    }

    std::string image_path = argv[1];
    uint64_t entry_number = std::stoull(argv[2]);
    uint64_t attribute_type = std::stoull(argv[3]);

    if (!read_image(image_path)) {
        std::cerr << "Failed to open image: " << image_path << std::endl;
        return 1;
    }
    if(!read_boot_sector(global_reader)) {
        std::cerr << "Failed to read boot sector" << std::endl;
        return 1;
    }

    print_data_attribute(entry_number, attribute_type);
    close_image();

    return 0;
}