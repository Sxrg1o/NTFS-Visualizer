#include "../../include/attrProc.h"
#include "../../include/structReader.h"
#include "../../include/read.h"
#include "../../include/data_structures.h"
#include "../../include/image.h"
#include "../../include/entryProc.h"
#include "../../include/HexPrinter.h"
#include <vector>
#include <cstdint>
#include <algorithm>

// Need to read $Data attribute for $AttrDef entry (4) to get the names and types of attributes
std::unordered_map<uint32_t, attrDefEntry> attrTypeNames = {};

std::unordered_map<uint32_t, std::string> dosPermFlags = {
    {STDINF_READONLY, "Read-only"},
    {STDINF_HIDDEN, "Hidden"},
    {STDINF_SYSTEM, "System"},
    {STDINF_ARCHIVE, "Archive"},
    {STDINF_DEVICE, "Device"},
    {STDINF_NORMAL, "Normal"},
    {STDINF_TEMPORARY, "Temporary"},
    {STDINF_SPARSE_FILE, "Sparse"},
    {STDINF_REPARSE_POINT, "Reparse"},
    {STDINF_COMPRESSED, "Compressed"},
    {STDINF_OFFLINE, "Offline"},
    {STDINF_NOT_CONTENT_INDEXED, "Not indexed"},
    {STDINF_ENCRYPTED, "Encrypted"}
};

volumeInfo volume_information = {};

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

mftAttr* find_attribute(mftEntry &entry, uint32_t type, uint32_t data_stream) {      
    if (data_stream == 0) {
        for (auto &attr : entry.attrs) {
            if (attr.header.type == type) {
                return &attr;
            }
        }
        return nullptr;
    }
    uint32_t stream_count = 1;
    for (auto &attr : entry.attrs) {
        if (attr.header.type == type) {
            if (stream_count == data_stream) {
                return &attr;
            }
            stream_count++;
        }
    }
    
    return nullptr;
}

dataAttr read_data_attribute(const std::unique_ptr<Reader>& reader, mftAttr* attr, uint64_t entry_number) {
    dataAttr result;
    uint64_t entry_offset = image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster + entry_number * 1024;
    result.is_resident = attr->header.resident_flag == ATTR_RESIDENT;

    if (result.is_resident) {
        const auto& res_data = attr->content.resident_attr;
        result.logical_size = res_data.size;
        result.resident_offset = attr->offset + res_data.offset;
    } else {
        const auto& non_res_data = attr->content.non_resident_attr;
        result.logical_size = non_res_data.actual_size;
        
        reader->seek(entry_offset + attr->offset + non_res_data.offset, false);
        
        uint64_t absolute_cluster_offset = 0;
        uint64_t total_clusters = (non_res_data.VCN_end - non_res_data.VCN_start + 1);
        uint64_t clusters_read = 0;

        while (clusters_read < total_clusters) {
            uint8_t header;
            reader->read(&header, sizeof(uint8_t));
            
            if (header == 0) break;

            uint8_t offset_length = (header & 0xF0) >> 4;
            uint8_t length_length = header & 0x0F;

            dataRun run;
            
            std::vector<uint8_t> length_bytes(length_length);
            reader->read(length_bytes.data(), length_length);
            run.cluster_count = read_var_length_number(length_bytes.data(), length_length);

            if (clusters_read + run.cluster_count > total_clusters) {
                run.cluster_count = total_clusters - clusters_read;
            }

            if (offset_length > 0) {
                std::vector<uint8_t> offset_bytes(offset_length);
                reader->read(offset_bytes.data(), offset_length);
                int64_t relative_offset = read_var_length_number(offset_bytes.data(), offset_length);
                absolute_cluster_offset += relative_offset;
                run.cluster_offset = absolute_cluster_offset;
                run.is_sparse = false;      
            } else {
                run.cluster_offset = 0;
                run.is_sparse = true;
            }

            result.runs.push_back(run);
            clusters_read += run.cluster_count;
        }
    }

    return result;
}

std::vector<uint8_t> read_data_portion(const std::unique_ptr<Reader>& reader, const dataAttr& attr, uint64_t chunk, uint64_t size, uint64_t entry_number) {
    std::vector<uint8_t> result;
    uint64_t offset = chunk * 200;
    
    uint64_t max_chunks = (attr.logical_size + size - 1) / size;
    if (chunk >= max_chunks) {
        chunk = max_chunks - 1;
    }

    uint64_t real_size = size;
    if(offset + real_size > attr.logical_size) {
        real_size = attr.logical_size - offset;
    }
    
    result.resize(real_size);
    uint64_t entry_offset = image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster + entry_number * 1024;
    
    if (attr.is_resident) {
        reader->seek(entry_offset + attr.resident_offset + offset, false);
        reader->read(result.data(), real_size);
    } else {
        uint64_t bytes_per_cluster = image.bytes_x_sector * image.sectors_x_cluster;
        uint64_t start_cluster = offset / bytes_per_cluster;
        uint64_t end_cluster = (offset + real_size + bytes_per_cluster - 1) / bytes_per_cluster;
        
        uint64_t current_cluster = 0;
        uint64_t bytes_read = 0;
        
        for (const auto& run : attr.runs) {
            if (current_cluster + run.cluster_count > start_cluster) {
                uint64_t cluster_offset = start_cluster - current_cluster;
                uint64_t run_offset = cluster_offset * bytes_per_cluster;
                uint64_t data_offset = offset - (start_cluster * bytes_per_cluster);
                
                uint64_t available_bytes = run.cluster_count * bytes_per_cluster - run_offset;
                uint64_t bytes_to_read = std::min(real_size - bytes_read, available_bytes);
                
                if (!run.is_sparse) {
                    uint64_t base_offset = run.cluster_offset * bytes_per_cluster + run_offset;
                    reader->seek(base_offset + data_offset, false);
                    reader->read(result.data() + bytes_read, bytes_to_read);
                } else {
                    std::fill_n(result.data() + bytes_read, bytes_to_read, 0);
                }
                
                bytes_read += bytes_to_read;
                if (bytes_read >= real_size) break;
            }
            current_cluster += run.cluster_count;
        }
    }
    
    return result;
}

std::string get_file_name(mftEntry entry) {
    mftAttr* filename_attr = find_attribute(entry, ATTR_FILENAME, 0);
    if (!filename_attr) {
        return "";  
    }

    uint64_t name_offset = 0;
    
    if (filename_attr->header.resident_flag == ATTR_RESIDENT) {
        name_offset = filename_attr->offset + filename_attr->content.resident_attr.offset;
        fileName file_name_struct;
        
        global_reader->seek(image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster + 
                          entry.number * 1024 + name_offset, false);
        
        if (!StructReader::read(file_name_struct, global_reader.get())) {
            return "";
        }
        
        std::vector<uint16_t> name_buffer(file_name_struct.name_length);
        global_reader->read(reinterpret_cast<uint8_t*>(name_buffer.data()), 
                          file_name_struct.name_length * sizeof(uint16_t));
        
        std::string file_name;
        for (uint16_t unicode_char : name_buffer) {
            if (unicode_char < 128) { 
                file_name += static_cast<char>(unicode_char);
            } else {
                file_name += '?';
            }
        }
        
        return file_name;
    }
    
    return "";  
}

/*** Processing specific attributes ***/

void get_attr_def_info() {
    mftEntry entry = read_mft_entry(global_reader, MDF_ATTRDEF);
    mftAttr *attr_def_attr = find_attribute(entry, ATTR_DATA, 0);
    dataAttr attr_def_data = read_data_attribute(global_reader, attr_def_attr, MDF_ATTRDEF);
    std::vector<uint8_t> data_portion = read_data_portion(global_reader, attr_def_data, 0, attr_def_data.logical_size, MDF_ATTRDEF);
    
    std::unordered_map<uint32_t, attrDefEntry> local_attrTypeNames;
    
    size_t num_entries = data_portion.size() / sizeof(attrDef);
    
    for (size_t i = 0; i < num_entries; i++) {
        attrDef def;
        size_t offset = i * sizeof(attrDef);
        
        if (!StructReader::from_bytes(def, std::vector<uint8_t>(
                data_portion.begin() + offset,
                data_portion.begin() + offset + sizeof(attrDef)))) {
            std::cerr << "Error reading $AttrDef" << i << std::endl;
            continue;
        }
        
        std::string attrName;
        for (int j = 0; j < 16; j++) {
            char16_t* chars = reinterpret_cast<char16_t*>(&def.name[j]);
            for (int k = 0; k < 4; k++) {  
                if (chars[k] == 0) break;
                if (chars[k] <= 127) {
                    attrName.push_back(static_cast<char>(chars[k]));
                }
            }
        }
        
        while (!attrName.empty() && (attrName.back() == ' ' || attrName.back() == '\0')) {
            attrName.pop_back();
        }
        
        if (attrName.empty()) continue;
        
        std::string finalName = attrName;
        if (!finalName.empty() && finalName[0] == '$') {
            finalName = finalName.substr(1);
        }
        
        attrDefEntry info;
        info.name = finalName;
        info.flags = def.flags;
        info.min_size = def.min_size;
        info.max_size = def.max_size;
        
        local_attrTypeNames[def.type] = info;
    }
    
    attrTypeNames = local_attrTypeNames;
}

// Depending on the version, some attributes have different data structures

void get_volume_info() {
    mftEntry entry = read_mft_entry(global_reader, MDF_VOL);
    mftAttr *volume_info_attr = find_attribute(entry, ATTR_VOLUME_INFO, 0);
    dataAttr volume_info_data = read_data_attribute(global_reader, volume_info_attr, MDF_VOL);
    std::vector<uint8_t> data_portion = read_data_portion(global_reader, volume_info_data, 0, volume_info_data.logical_size, MDF_VOL);    

    volumeInfo local_volume_info = {};
    if (!StructReader::from_bytes(local_volume_info, data_portion)) {
        std::cerr << "Error reading Volume_Information structure" << std::endl;
        return;
    }

    volume_information = local_volume_info;
}

std::string get_attr_name(uint32_t type) {
    auto it = attrTypeNames.find(type);
    if (it != attrTypeNames.end()) {
        return it->second.name;
    }
    std::stringstream ss;
    ss << "0x" << std::hex << std::uppercase << type;
    return ss.str();
}

std::string get_file_permissions_string(uint32_t perms) {
    std::string result;
    for (const auto& [flag, name] : dosPermFlags) {
        if (perms & flag) {
            if (!result.empty()) result += " ";
            result += name;
        }
    }
    return result.empty() ? "None" : result;
}

// Processes $STANDARD_INFORMATION attribute
standardInfo get_stdinf_data(mftAttr* attr, uint64_t entry_number) {
    dataAttr data = read_data_attribute(global_reader, attr, 0);
    auto bytes = read_data_portion(global_reader, data, 0, data.logical_size, entry_number);

    standardInfo result;
    if(!StructReader::from_bytes(result, bytes)) {
        std::cerr << "Error reading vector of bytes" << std::endl; 
        return {};
    }

    return result;
}