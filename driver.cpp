#include "driver.hpp"
#include <cstring>

bool Driver::vGpuMemcpy(void* host_ptr, size_t size, MemcpyKind kind) {
    if (!host_ptr || size == 0) return false;

    if (kind == MemcpyKind::HostToDevice) {

        unsigned char* current_dest = dev->getVramPointer();

        if (!dev->canReserveVram(size)) return false; // if not enough VRAM, fail

        dev->setCurrentGPUState(GPUstate::Processing); 

        std::memcpy(current_dest, host_ptr, size); 
        std::cout << "[DRIVER] Copied " << size << " bytes to Device. \n";
        std::cout << "[DEBUG] VRAM_Zero_Ptr: " << (void*)dev->getVramStartPointer() << " , VRAM_Top_Ptr: " << (void*)dev->getVramPointer() << " , Calculated Top_Ptr: " << (void*)(dev->getVramStartPointer() + size) << " \n";

    }
    else if (kind == MemcpyKind::DeviceToHost) {
        dev->setCurrentGPUState(GPUstate::Processing);

        unsigned char* current_dest = dev->getVramPointer();

        std::memcpy(host_ptr, current_dest-size, size);
        std::cout << "[DRIVER] Copied " << size << " bytes back to Host. \n";

    }

    dev->setCurrentGPUState(GPUstate::Idle);
    return true;
}