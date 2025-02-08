#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>

typedef struct {
    uint16_t bytes_x_sector;
    uint8_t sectors_x_cluster;
    uint64_t cluster_MFT_start;
    uint64_t sectors_x_volume;
    uint8_t entry_size;
    uint8_t index_size;
} ntfsImage;

#endif 