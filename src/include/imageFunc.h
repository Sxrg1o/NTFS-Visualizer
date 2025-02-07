#ifndef IMAGE_FUNC_H
#define IMAGE_FUNC_H

#include <string>
#include <memory>
#include <pybind11/pybind11.h>

class Reader;

namespace py = pybind11;

bool read_image(const std::string& filePath);
void close_image();

extern std::unique_ptr<Reader> global_reader;

#endif