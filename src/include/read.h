#pragma once
#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class Reader {
public:
    virtual ~Reader() = default;
    
    // Opens the resource and returns true if it succeeded
    virtual bool open(const std::string& path) = 0;
    
    // Closes the resource
    virtual void close() = 0;
    
    // Moves the cursor relative to the current position
    // Returns the new position or -1 in case of error
    virtual int64_t seek(int64_t offset) = 0;
    
    // Reads size bytes and stores them in buffer
    // Returns the number of bytes read or -1 in case of error
    virtual int64_t read(uint8_t* buffer, size_t size) = 0;
    
    // Gets the current position of the cursor
    virtual int64_t tell() const = 0;
};


class ImageReader : public Reader {
private:
    std::fstream file;
    int64_t currentPos;

public:
    ImageReader() : currentPos(0) {}
    
    bool open(const std::string& path) override {
        file.open(path, std::ios::binary | std::ios::in);
        currentPos = 0;
        return file.is_open();
    }
    
    void close() override {
        file.close();
        currentPos = 0;
    }
    
    int64_t seek(int64_t offset) override { // Absolute position 
        file.seekg(offset, std::ios::beg);
        if (file.fail()) {
            return -1;
        }
        currentPos = offset;
        return currentPos;
    }
    
    int64_t read(uint8_t* buffer, size_t size) override {
        if (!file.is_open()) {
            std::cerr << "Stream not open" << std::endl;
            return -1;
        }
        
        file.read(reinterpret_cast<char*>(buffer), size);
        
        if (file.fail() && !file.eof()) {
            std::cerr << "Error reading file" << std::endl;
            return -1;
        }
        
        auto bytesRead = file.gcount();
        currentPos += bytesRead;
        return bytesRead;
    }
    
    int64_t tell() const override {
        return currentPos;
    }
};


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

class ReaderFactory {
public:
    static std::unique_ptr<Reader> createReader(bool isPartition) {
        if (isPartition) {
            return std::make_unique<PartitionReader>();
        }
        return std::make_unique<ImageReader>();
    }
};