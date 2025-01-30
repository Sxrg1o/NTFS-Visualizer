#include <pybind11/pybind11.h>
#include "../include/imageFunc.h"

namespace py = pybind11;

PYBIND11_MODULE(bindings, m) {
    m.def("read_image", &readImage, "A function to read an NTFS image and return boot sector data");
    m.def("close_image", &closeImage, "A function to close the image file");
}