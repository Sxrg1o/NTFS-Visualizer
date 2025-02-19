#ifndef ATTR_PROC_H
#define ATTR_PROC_H

//#include <pybind11/pybind11.h>
#include "data_structures.h"
#include "read.h"
#include "image.h"
#include "entryProc.h"
#include "clusterProc.h"
#include <unordered_map>

//namespace py = pybind11;

typedef struct {
    int64_t cluster_offset;
    uint64_t cluster_count;
    bool is_sparse;
} dataRun;

typedef struct {
    bool is_resident;
    uint64_t resident_offset;
    uint64_t logical_size;
    std::vector<dataRun> runs;
} dataAttr;

typedef struct {
    std::string name;
    uint32_t flags;
    uint64_t min_size;
    uint64_t max_size;
} attrDefEntry;

uint64_t read_var_length_number(const uint8_t*, uint8_t);
mftAttr* find_attribute(mftEntry &, uint32_t, uint32_t);
dataAttr read_data_attribute(const std::unique_ptr<Reader>&, mftAttr*, uint64_t);
std::vector<uint8_t> read_data_portion(const std::unique_ptr<Reader>&, const dataAttr&, uint64_t, uint64_t, uint64_t);
std::string get_file_name(mftEntry);
standardInfo get_stdinf_data(mftAttr*, uint64_t);
std::string get_attr_name(uint32_t);
std::string get_file_permissions_string(uint32_t);
void get_attr_def_info();
void get_volume_info();

extern std::unique_ptr<Reader> global_reader;
extern ntfsImage image;

#endif