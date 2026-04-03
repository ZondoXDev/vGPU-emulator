#include <iostream>
#include "device.hpp"

int main() {
    std::cout<<"vGPU Started v0.1"<<std::endl;
    DeviceDescriptor intel_gpu_config;
    intel_gpu_config.device_name = "Skylake Graphics (Gen9)";
    intel_gpu_config.vendor_name = "Intel";
    intel_gpu_config.litography = 14; 
    intel_gpu_config.memory_size = 1024 * 128;
    intel_gpu_config.architecture = "super_arch1";
    intel_gpu_config.core_clock_mhz = 3200;
    intel_gpu_config.execution_units = 4;
    

    Device my_gpu(intel_gpu_config);

    my_gpu.printStatus();

    std::cout<<"SIM Stopped"<<std::endl;

    return 0;
}