#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#include <unistd.h>
#endif

class NTFSImageHandler {
private:
    std::fstream imageFile;
    const size_t SECTOR_SIZE = 512;
    const size_t MFT_RECORD_SIZE = 1024;

    struct PartitionEntry {
        uint8_t status;
        uint8_t start_head;
        uint16_t start_sector_cylinder;
        uint8_t partition_type;
        uint8_t end_head;
        uint16_t end_sector_cylinder;
        uint32_t start_sector;
        uint32_t total_sectors;
    };

public:
    bool readImage(const std::string& imagePath) {
        imageFile.open(imagePath, std::ios::binary | std::ios::in);
        if (!imageFile.is_open()) {
            std::cerr << "Error at reading image" << imagePath << std::endl;
            return false;
        }
        
        char signature[8];
        imageFile.seekg(3);
        imageFile.read(signature, 8);
        
        if (std::strncmp(signature, "NTFS    ", 8) != 0) {
            std::cerr << "Invalid NTFS image" << std::endl;
            imageFile.close();
            return false;
        }
        
        return true;
    }

    bool readPartition(const std::string& devicePath) {
        imageFile.open(devicePath, std::ios::binary | std::ios::in);
        if (!imageFile.is_open()) {
            std::cerr << "Error at reading partition" << devicePath << std::endl;
            return false;
        }

        std::vector<uint8_t> mbr(SECTOR_SIZE);
        imageFile.read(reinterpret_cast<char*>(mbr.data()), SECTOR_SIZE);

        for (int i = 0; i < 4; i++) {
            PartitionEntry* entry = reinterpret_cast<PartitionEntry*>(&mbr[446 + i * 16]);
            if (entry->partition_type == 0x07) {
                imageFile.seekg(entry->start_sector * SECTOR_SIZE);
                return true;
            }
        }

        std::cerr << "NTFS Partition not found" << std::endl;
        return false;
    }

    bool createImage(const std::string& imagePath, size_t sizeInMB) {
        #ifdef _WIN32
        std::ofstream file(imagePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error creating image" << std::endl;
            return false;
        }
        
        file.seekp((sizeInMB * 1024 * 1024) - 1);
        file.put('\0');
        file.close();
        
        std::string cmd = "format " + imagePath + " /FS:NTFS /Q /Y";
        return (system(cmd.c_str()) == 0);
        
        #else
        std::string ddCmd = "dd if=/dev/zero of=" + imagePath + 
                        " bs=1M count=" + std::to_string(sizeInMB);
        if (system(ddCmd.c_str()) != 0) {
            std::cerr << "Error creating image" << std::endl;
            return false;
        }
        
        std::string mkfsCmd = "mkfs.ntfs -F " + imagePath;
        if (system(mkfsCmd.c_str()) != 0) {
            std::cerr << "Error at formatting image" << std::endl;
            return false;
        }
        
        return true;
        #endif
    }

    ~NTFSImageHandler() {
        if (imageFile.is_open()) {
            imageFile.close();
        }
    }
};

int main() {
    NTFSImageHandler handler;
    int opcion;
    std::string path;

    std::cout << "1. Read NTFS image\n";
    std::cout << "2. Read NTFS partition\n";
    std::cout << "3. Create NTFS image\n";
    std::cout << "Select an option: ";
    std::cin >> opcion;
    std::cin.ignore();

    switch (opcion) {
        case 1: {
            std::cout << "Enter image path: ";
            std::getline(std::cin, path);
            if (handler.readImage(path)) {
                std::cout << "Image read successfully\n";
            }
            break;
        }
        case 2: {
            std::cout << "Enter partition path: ";
            std::getline(std::cin, path);
            if (handler.readPartition(path)) {
                std::cout << "Partition read successfully\n";
            }
            break;
        }
        case 3: {
            size_t size;
            std::cout << "Enter new image path: ";
            std::getline(std::cin, path);
            std::cout << "Enter size in MB: ";
            std::cin >> size;
            if (handler.createImage(path, size)) {
                std::cout << "Image created successfully\n";
            }
            break;
        }
        default:
            std::cout << "Opción inválida\n";
    }

    return 0;
}