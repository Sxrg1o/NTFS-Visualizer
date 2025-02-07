#ifndef ENTRY_PROC_H
#define ENTRY_PROC_H

#include <pybind11/pybind11.h>
#include "data_structures.h"
#include "read.h"
#include "image.h"

namespace py = pybind11;

typedef struct {
    mftAttrHeader header;
    union {
        residentData resident_attr;
        non_residentData non_resident_attr;
    } content;
} mftAttr;

typedef struct {
    mftEntryHeader header;
    std::vector<mftAttr> attrs;
} mftEntry;

mftAttr read_attribute(const std::unique_ptr<Reader>&);
mftEntry read_mft_entry(const std::unique_ptr<Reader>&, uint64_t);

extern std::unique_ptr<Reader> global_reader;
extern ntfsImage image;

#endif