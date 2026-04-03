#pragma once
#include <iostream>
#include <cstddef>
#include <cstdint>

enum class GPUstate {
    Offline,
    Idle,
    Processing,
    Overheated
};

struct DeviceDescriptor {
    std::string device_name;
    std::string vendor_name;
    std::string architecture;
    uint8_t litography;
    uint32_t execution_units;
    uint32_t core_clock_mhz;
    size_t memory_size;
};

class Device {
    private: 

        // Constant data from Device Descriptor
        std::string device_name;
        std::string vendor_name;
        std::string architecture;
        uint8_t litography;
        uint32_t execution_units;
        uint32_t core_clock_mhz;
        size_t memory_size;


        // Dynamic data
        GPUstate current_state;
        size_t used_vram_bytes;
        uint8_t current_temp_c;


        // Raw memory
        unsigned char* vram_buffer;

    public:
        // Device(std::string name, std::string vendor_name, uint8_t litography, size_t size_in_bytes);
        Device (const DeviceDescriptor& desc);
        ~Device();
        void printStatus();
};