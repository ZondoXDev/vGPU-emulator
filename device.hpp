#pragma once
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>

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
        void memoryDump();

        // Getters for the driver
        unsigned char* getVramStartPointer() { return vram_buffer; }
        unsigned char* getVramPointer() { return vram_buffer + used_vram_bytes; }
        size_t getMemorySize() const { return memory_size; }
        size_t getUsedMemory() const { return used_vram_bytes; }
        uint8_t getCurrentTemp() const { return current_temp_c; }

        // State management
        bool canReserveVram(size_t size); // Checks if there's enough free space in VRAM and allocates it if so
        void resetVram();
        void setCurrentGPUState(GPUstate state);

        void executeKernel(std::function<void(unsigned char*, size_t)> kernel);



};