#ifndef CLUSTER_PROC_H
#define CLUSTER_PROC_H

#include <memory>
//#include <pybind11/pybind11.h>
#include "data_structures.h"
#include "read.h"
#include "image.h"

//namespace py = pybind11;

struct ClusterStatus {
    enum Type {
        FREE = 0,
        USED = 1,
        BAD = 2
    };
    std::vector<Type> clusters;
};

bool read_boot_sector(const std::unique_ptr<Reader>&);
//py::dict boot_sector_dict();
std::string boot_sector_hex();
ClusterStatus analyze_clusters(uint64_t);

extern std::unique_ptr<Reader> global_reader;

#endif