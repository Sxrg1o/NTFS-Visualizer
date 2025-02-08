#include "../../include/attrProc.h"
#include "../../include/structReader.h"
#include "../../include/read.h"
#include "../../include/data_structures.h"
#include "../../include/image.h"
#include "../../include/entryProc.h"
#include "../../include/HexPrinter.h"
#include <vector>
#include <cstdint>

uint64_t read_var_length_number(const uint8_t* data, uint8_t size) {
    uint64_t result = 0;
    for (int i = size - 1; i >= 0; i--) {
        result = (result << 8) | data[i];
    }
    if (size > 0 && (data[size - 1] & 0x80)) {
        for (size_t i = size; i < 8; i++) {
            result |= (0xFFULL << (i * 8));
        }
    }
    return result;
}

mftAttr* find_attribute(mftEntry &entry, uint32_t type) {
    for(auto &attr : entry.attrs) {
        if(attr.header.type == type) {
            return &attr;
        }
    }
    return nullptr;
}

dataAttr read_data_attribute(const std::unique_ptr<Reader>& reader, mftAttr* attr, uint64_t entry_number) {
    dataAttr result;
    result.is_resident = attr->header.resident_flag == ATTR_RESIDENT;

    if (result.is_resident) {   // Handle resident attribute
        const auto& res_data = attr->content.resident_attr;
        result.logical_size = res_data.size;
        reader->seek(image.cluster_MFT_start + entry_number * image.entry_size + res_data.offset, false);
        result.data.resize(res_data.size);
        reader->read(result.data.data(), res_data.size);
    } else {                   // Handle non-resident attribute   
        const auto& non_res_data = attr->content.non_resident_attr;
        result.logical_size = non_res_data.actual_size;
        
        if (non_res_data.alloc_size > 0) {
            result.data.reserve(non_res_data.alloc_size);
        }

        reader->seek(image.cluster_MFT_start + entry_number * image.entry_size + non_res_data.offset, false);

        uint64_t absolute_cluster_offset = 0;
        uint64_t total_clusters = (non_res_data.VCN_end - non_res_data.VCN_start + 1);
        uint64_t clusters_read = 0;
        uint64_t run_list_pos = reader->tell();  

        while (clusters_read < total_clusters) {
            uint8_t header;
            reader->read(&header, 1);

            if (header == 0) break;

            uint8_t offset_length = (header & 0xF0) >> 4;
            uint8_t length_length = header & 0x0F;

            dataRun run;

            std::vector<uint8_t> length_bytes(length_length);
            reader->read(length_bytes.data(), length_length);
            run.cluster_count = read_var_length_number(length_bytes.data(), length_length);

            if (clusters_read + run.cluster_count > total_clusters) {   // Adjust count if needed
                run.cluster_count = total_clusters - clusters_read;
            }

            // Read offset if it exists
            if (offset_length > 0) {
                std::vector<uint8_t> offset_bytes(offset_length);
                reader->read(offset_bytes.data(), offset_length);
                int64_t relative_offset = read_var_length_number(offset_bytes.data(), offset_length);
                absolute_cluster_offset += relative_offset;
                run.cluster_offset = absolute_cluster_offset;
                run.is_sparse = (relative_offset == 0);
            } else {
                run.cluster_offset = 0;
                run.is_sparse = true;
            }

            result.runs.push_back(run);

            uint64_t run_size = run.cluster_count * image.bytes_x_sector * image.sectors_x_cluster;
            uint64_t current_pos = result.data.size();
            result.data.resize(current_pos + run_size);

            if (!run.is_sparse) {
                uint64_t base_offset = run.cluster_offset * image.bytes_x_sector * image.sectors_x_cluster;
                uint64_t return_pos = reader->tell();
                
                reader->seek(base_offset, false);
                reader->read(result.data.data() + current_pos, run_size);
                
                reader->seek(return_pos, false);    // Return to run list
            }
            /** For sparse (filled with zeros) runs, the resize already filled with zeros **/ 

            clusters_read += run.cluster_count;
        }

        if (result.data.size() > non_res_data.actual_size) {
            result.data.resize(non_res_data.actual_size);
        }
    }

    return result;
}