#pragma once
#include "Reader.h"
#include <fstream>
#include <iostream>

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
    
    int64_t seek(int64_t offset) override {
        file.seekg(offset, std::ios::cur);
        if (file.fail()) {
            return -1;
        }
        currentPos += offset;
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