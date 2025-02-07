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
    uint64_t offset = image.cluster_MFT_start + number * image.entry_size;
    reader->seek(offset, false);    // Seek to the entry 
    if (!StructReader::read(header, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return {};
    }
    entry.header = header;
    reader->seek(header.first_attr_off, true);  // Seek to the first attribute
    offset = header.first_attr_off;             // Save the offset

    while(true) {
        mftAttr attr = read_attribute(reader);
        reader->seek(offset, false);                // Back to the beginning of attribute
        reader->seek(attr.header.length, true);     // Seek to the end of attribute
        entry.attrs.push_back(attr);
        offset += attr.header.length;               // Save the offset
        // Check if it's the last attribute
        uint32_t marker;
        reader->read(reinterpret_cast<uint8_t*>(&marker), sizeof(uint32_t));
        if(marker == END_OF_ENTRY) break;
        reader->seek(-sizeof(uint32_t), true);      // Back to the beginning of the new attribute
    }
    return entry;
}
