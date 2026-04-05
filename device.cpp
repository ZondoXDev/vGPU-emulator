#include "device.hpp"
Device::Device(const DeviceDescriptor& desc){
    device_name = desc.device_name;
    vendor_name = desc.vendor_name;
    architecture = desc.architecture;
    memory_size = desc.memory_size;
    litography = desc.litography;
    execution_units = desc.execution_units;
    core_clock_mhz = desc.core_clock_mhz;
    memory_size = desc.memory_size;

    // dynamic values
    current_state = GPUstate::Idle;
    used_vram_bytes = 0;
    current_temp_c = 35;


    vram_buffer = new unsigned char[memory_size];

    std::cout<<"[DEVICE] New GPU initialized! " << vendor_name << " " << device_name << " with " << memory_size << " bytes of VRAM! \n";
    
}

Device::~Device() {
    delete[] vram_buffer;
    vram_buffer = nullptr;
    std::cout<<"[DEVICE]: GPU OFF: " << device_name << " Memory flushed!" << std::endl;

}
void Device::printStatus() {
    std::cout << "[DEVICE STATUS]: \n";
    std::cout << "Vendor: "<< vendor_name << "\n";
    std::cout << "Model: "<< device_name << "\n";
    std::cout << "Architecture: "<< architecture << "\n";
    std::cout << "Memory size: "<< memory_size << "\n";
    std::cout << "Litography: "<< static_cast<int>(litography) << "\n";
    std::cout << "Execution Units: "<< execution_units << "\n";
    std::cout << "Core Clock Frequency: "<< core_clock_mhz << "\n";

}

bool Device::canReserveVram(size_t size) {
    if(used_vram_bytes + size >= memory_size) {
        std::cerr << "[DEVICE ERROR] Out of Memory! Requested: " << size << " bytes. \n";
        return false;
    }
    used_vram_bytes += size;
    return true;
}

void Device::setCurrentGPUState(GPUstate state) {
    current_state = state;
}

void Device::resetVram() {
    used_vram_bytes = 0;
    if (vram_buffer) {
        std::memset(vram_buffer, 0, memory_size);
    }
}

 void Device::memoryDump() {
    std::cout << "[vGPU Memory Dump]: \n";

    int* int_ptr = reinterpret_cast<int*>(vram_buffer);
    size_t num_ints = used_vram_bytes / sizeof(int);

    for (size_t i = 0; i < num_ints; i++) {
        std::cout << "[" << (void*)(int_ptr + i) << " : " << int_ptr[i] << "]\n";
    }

    std::cout << "\n[Memory Dumped] \n";
 }

 void Device::executeKernel(std::function<void(unsigned char*, size_t)> kernel) {
    if (used_vram_bytes == 0) {
        std::cout << "VRAM Empty. Nothing to execute\n";
        return;
    }

    setCurrentGPUState(GPUstate::Processing);
    std::cout << "[DEVICE] Exectuing compute kernel...\n";

    kernel(vram_buffer, used_vram_bytes);

    setCurrentGPUState(GPUstate::Idle);
    std::cout << "[DEVICE] Compute kernel finished.\n";
    
 }