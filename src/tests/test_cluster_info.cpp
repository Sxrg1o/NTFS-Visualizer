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

void print_data_attribute(uint64_t entry_number) {
    if (!global_reader) {
        std::cerr << "No file/partition opened" << std::endl;
        return;
    }

    mftEntry entry = read_mft_entry(global_reader, entry_number);
    mftAttr* data_attr = find_attribute(entry, ATTR_DATA, 0);  // Just for testing
    if (!data_attr) {
        std::cerr << "No attribute found in entry " << entry_number << std::endl;
        return;
    }

    dataAttr data = read_data_attribute(global_reader, data_attr, entry_number);

    std::cout << "Entry number: " << entry_number << std::endl;
    std::cout << "Attribute type: $DATA" << std::endl;      // Just for testing
    std::cout << "Resident: " << (data.is_resident ? "Yes" : "No") << std::endl;
    std::cout << "Logical size: " << data.logical_size << " bytes" << std::endl;
    std::cout << "Data size: " << data.data.size() << " bytes" << std::endl;

    /*std::cout << "\nData content (hex):\n";
    for (size_t i = 0; i < data.data.size(); ++i) {
        if (i > 0 && i % 16 == 0) std::cout << std::endl;
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(data.data[i]) << " ";
    }
    std::cout << std::dec << std::endl;*/

    if (!data.is_resident && !data.runs.empty()) {
        std::cout << "\nRun list information:\n";
        for (const auto& run : data.runs) {
            std::cout << "VCN Start/End: " << data_attr->content.non_resident_attr.VCN_start << "/" << data_attr->content.non_resident_attr.VCN_end << std::endl;
            std::cout << "Non-resident data offset: " << data_attr->content.non_resident_attr.offset << std::endl;
            std::cout << " Cluster count: " << run.cluster_count 
                      << ", Offset: " << run.cluster_offset 
                      << ", Sparse: " << (run.is_sparse ? "Yes" : "No") << std::endl;
        }
    }

    /*ClusterStatus status = analyze_clusters(0);
    std::cout << "Cluster status:\n";
    int i = 0;
    for(auto &cluster : status.clusters) {
        if(i % 20 == 0 && i != 0) std::cout << std::endl;
        std::cout << cluster << " ";
        i++;
    }
    std::cout << std::endl;*/

}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <image_path> <entry_number>" << std::endl;
        return 1;
    }

    std::string image_path = argv[1];
    uint64_t entry_number = std::stoull(argv[2]);

    if (!read_image(image_path)) {
        std::cerr << "Failed to open image: " << image_path << std::endl;
        return 1;
    }
    if(!read_boot_sector(global_reader)) {
        std::cerr << "Failed to read boot sector" << std::endl;
        return 1;
    }

    print_data_attribute(entry_number);
    close_image();

    return 0;
}