#include <pybind11/pybind11.h>
#include "../include/imageFunc.h"
#include "../include/clusterProc.h"

namespace py = pybind11;

extern "C" {
    PYBIND11_MODULE(bindings, m) {

        // Essentials
        m.def("read_image", &read_image, "A function to read an NTFS image");
        m.def("close_image", &close_image, "A function to close the image file");

        // Boot Sector
        m.def("boot_sector_dict", &boot_sector_dict, "A function to convert the boot sector to a dictionary");
        m.def("boot_sector_hex", &boot_sector_hex, "A function to print the boot sector in hexadecimal format");

    }
}
