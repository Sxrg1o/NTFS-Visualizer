#ifndef IMAGE_FUNC_HPP
#define IMAGE_FUNC_HPP

#include <string>
#include <memory>
#include <pybind11/pybind11.h>

class Reader;

namespace py = pybind11;

py::dict readImage(const std::string& filePath);
void closeImage();

extern std::unique_ptr<Reader> global_reader;

#endif