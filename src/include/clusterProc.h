#ifndef CLUSTER_PROC_HPP
#define CLUSTER_PROC_HPP

#include <memory>
#include "data_structures.h"
#include "../core/imageReader/Reader.h"

bootSector readBootSector(const std::unique_ptr<Reader>&);

#endif