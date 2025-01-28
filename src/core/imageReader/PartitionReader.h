#pragma once
#include "Reader.h"
#include <fcntl.h>
#include <unistd.h>

class PartitionReader : public Reader {
private:
    int fd;
    int64_t currentPos;

public:
    PartitionReader() : fd(-1), currentPos(0) {}
    
    bool open(const std::string& path) override {
        fd = ::open(path.c_str(), O_RDONLY);
        currentPos = 0;
        return fd != -1;
    }
    
    void close() override {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
        currentPos = 0;
    }
    
    int64_t seek(int64_t offset) override {
        auto newPos = ::lseek(fd, offset, SEEK_CUR);
        if (newPos == -1) {
            return -1;
        }
        currentPos = newPos;
        return currentPos;
    }
    
    int64_t read(uint8_t* buffer, size_t size) override {
        auto bytesRead = ::read(fd, buffer, size);
        if (bytesRead > 0) {
            currentPos += bytesRead;
        }
        return bytesRead;
    }
    
    int64_t tell() const override {
        return currentPos;
    }
};