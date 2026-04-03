#pragma once
#include "device.hpp"

enum class MemcpyKind {
    HostToDevice,
    DeviceToHost
};

class Driver {
    private:
        Device* dev;

    public:
        Driver(Device* device) : dev(device) {}

        bool vGpuMemcpy(void* host_ptr, size_t size, MemcpyKind kind);

};