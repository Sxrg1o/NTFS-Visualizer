#include "../../include/structReader.h"
#include "../../include/read.h"
#include "../../include/data_structures.h"
#include "../../include/image.h"
#include "../../include/entryProc.h"
#include "../../include/HexPrinter.h"
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
    uint64_t offset = image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster + number * image.entry_size;
    reader->seek(offset, false);
    if (!StructReader::read(header, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return {};
    }
    entry.header = header;
    reader->seek(offset + header.first_attr_off, false); 
    offset += header.first_attr_off;
     
    while(true) {
        mftAttr attr = read_attribute(reader);
        // Check if it's the last attribute
        if(attr.header.type == END_OF_ENTRY) break;
        reader->seek(offset + attr.header.length, false);
        entry.attrs.push_back(attr);
        offset += attr.header.length;
    }
    return entry;
}
