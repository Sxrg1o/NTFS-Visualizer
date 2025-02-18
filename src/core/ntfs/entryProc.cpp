#include "../../include/structReader.h"
#include "../../include/read.h"
#include "../../include/data_structures.h"
#include "../../include/image.h"
#include "../../include/entryProc.h"
#include "../../include/HexPrinter.h"
#include "../../include/clusterProc.h"
#include "../../include/attrProc.h"
#include <vector>
#include <cstdint>

mftAttr read_attribute(const std::unique_ptr<Reader>& reader) {
    mftAttr attr;
    mftAttrHeader header;

    if (!StructReader::read(header, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return {};
    }
    if(header.resident_flag == ATTR_RESIDENT) {
        if(!StructReader::read(attr.content.resident_attr, reader.get())) {
            std::cerr << "Error reading file/partition" << std::endl; 
            return {};
        }
    } else {
        if(!StructReader::read(attr.content.non_resident_attr, reader.get())) {
            std::cerr << "Error reading file/partition" << std::endl; 
            return {};
        }
    }
    attr.header = header;
    return attr;
}

mftEntry read_mft_entry(const std::unique_ptr<Reader>& reader, uint64_t number) {
    mftEntry entry;
    mftEntryHeader header;
    // Offset used for attribute reading
    uint64_t offset = image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster + number * 1024;
    uint64_t size;
    entry.number = number;
    reader->seek(offset, false);
    if (!StructReader::read(header, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return {};
    }
    entry.header = header;
    reader->seek(offset + header.first_attr_off, false); 
    offset += header.first_attr_off;
    size = header.first_attr_off;

    while(true) {
        mftAttr attr = read_attribute(reader);
        attr.offset = offset - image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster - number * 1024;
        // Check if it's the last attribute
        if(attr.header.type == END_OF_ENTRY) break;
        reader->seek(offset + attr.header.length, false);
        entry.attrs.push_back(attr);
        offset += attr.header.length;
        size += attr.header.length;
    }
    entry.free_space = 1024 - size;
    //Print attributes 
    /*for(auto &attr : entry.attrs) {
        printf("Type: %u\n", attr.header.type);
        printf("Length: %u\n", attr.header.length);
        printf("Resident: %u\n", attr.header.resident_flag);
        printf("Name length: %u\n", attr.header.name_length);
        printf("Name offset: %u\n", attr.header.name_off);
        printf("Flags: %u\n", attr.header.flags);
        printf("Attr ID: %u\n", attr.header.attr_id);
    }*/

    return entry;
}

// Check allocation status of MFT entries
std::vector<bool> get_entries_status(uint64_t start, uint64_t count) {
    mftEntry mft_entry = read_mft_entry(global_reader, 0);
    mftAttr *bitmap_attr = find_attribute(mft_entry, ATTR_BITMAP, 0);
    dataAttr bitmap_data = read_data_attribute(global_reader, bitmap_attr, 0);
    
    uint64_t start_byte = start / 8;
    uint64_t chunk = start_byte / 200;
    auto bitmap_chunk = read_data_portion(global_reader, bitmap_data, chunk * 200, 200, 0);
    
    std::vector<bool> status;
    uint64_t bit_offset = start % 8;
    
    for (uint64_t i = 0; i < count && (i + start) < bitmap_data.logical_size * 8; i++) {
        uint64_t absolute_bit = start + i;
        uint64_t byte_needed = absolute_bit / 8;
        uint64_t chunk_needed = byte_needed / 200;      
        
        if (chunk_needed != chunk) {
            chunk = chunk_needed;
            bitmap_chunk = read_data_portion(global_reader, bitmap_data, chunk * 200, 200, 0);
        }
        
        uint64_t byte_in_chunk = byte_needed % 200;
        uint64_t bit_in_byte = absolute_bit % 8;
        
        if (byte_in_chunk < bitmap_chunk.size()) {
            status.push_back((bitmap_chunk[byte_in_chunk] & (1 << bit_in_byte)) != 0);
        }
    }
    
    return status;
}

// Read 10 by 10 entries
std::vector<mftEntry> read_entries(const std::unique_ptr<Reader>& reader, uint64_t start) {
    auto status = get_entries_status(start, 10);
    std::vector<mftEntry> entries;
    for(uint64_t i = 0; i < status.size(); i++) {
        if (status[i]) {
            entries.push_back(read_mft_entry(reader, start + i));
        }
    }
    return entries;
}



