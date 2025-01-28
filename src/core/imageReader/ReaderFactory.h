#pragma once
#include "ImageReader.h"
#include "PartitionReader.h"

class ReaderFactory {
public:
    static std::unique_ptr<Reader> createReader(bool isPartition) {
        if (isPartition) {
            return std::make_unique<PartitionReader>();
        }
        return std::make_unique<ImageReader>();
    }
};