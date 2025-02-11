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
    uint64_t offset = image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster + number * 1024;
    printf("Entry size: %x\n", image.entry_size);
    printf("MFT Entry start: %x\n", image.cluster_MFT_start);
    reader->seek(offset, false);
    if (!StructReader::read(header, reader.get())) {
        std::cerr << "Error reading file/partition" << std::endl; 
        return {};
    }
    entry.header = header;
    reader->seek(offset + header.first_attr_off, false); 
    offset += header.first_attr_off;
    
    printf("Entry header:\n");
    printf("Signature: %u\n", header.signature);
    printf("Link count: %u\n", header.link_count);
    printf("Attr offset: %u\n", header.first_attr_off);
    printf("Flags: %u\n", header.flags);

    while(true) {
        mftAttr attr = read_attribute(reader);
        attr.offset = offset - image.cluster_MFT_start * image.bytes_x_sector * image.sectors_x_cluster - number * 1024;
        // Check if it's the last attribute
        if(attr.header.type == END_OF_ENTRY) break;
        reader->seek(offset + attr.header.length, false);
        entry.attrs.push_back(attr);
        offset += attr.header.length;
    }
    /* Print attributes 
    for(auto &attr : entry.attrs) {
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
