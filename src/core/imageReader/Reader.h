#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <system_error>
#include <vector>
#include "HexPrinter.h"
#include <cstdint>

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

    void readAndPrintHex(size_t size, size_t maxLines = 0) {
        std::vector<uint8_t> buffer(size);
        auto bytesRead = read(buffer.data(), size);
        
        if (bytesRead > 0) {
            if (maxLines > 0) {
                HexPrinter::printLines(buffer.data(), bytesRead, maxLines, tell() - bytesRead);
            } else {
                HexPrinter::print(buffer.data(), bytesRead, tell() - bytesRead);
            }
        }
    }
};