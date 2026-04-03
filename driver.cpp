#include "driver.hpp"
#include <cstring>

bool Driver::vGpuMemcpy(void* host_ptr, size_t size, MemcpyKind kind) {
    if (!host_ptr || size == 0) return false;

    if (kind == MemcpyKind::HostToDevice) {
        if (!dev->canReserveVram(size)) return false;
        dev->setState(GPUstate::Processing);
        std::memcpy(dev->getVramPointer(), host_ptr, size);
        std::cout << "[DRIVER] Copied " << size << " bytes to Device. \n";

    }
    else {
        dev->setState(GPUstate::Processing);
        std::memcpy(host_ptr, dev->getVramPointer(), size);
        std::cout << "[DRIVER] Copied " << size << " bytes back to Host. \n";

    }

    dev->setState(GPUstate::Idle);
    return true;
}