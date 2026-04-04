#include <iostream>
#include <vector>
#include "device.hpp"
#include "driver.hpp"

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

    Driver my_driver(&my_gpu);
    std::vector<int> cpu_data_1 = {1, 2, 3, 4, 5};
    std::vector<int> cpu_data_2 = {12, 13, 14, 15, 16};

    size_t bytes_package_1 = cpu_data_1.size() * sizeof(int);
    size_t bytes_package_2 = cpu_data_2.size() * sizeof(int);
    my_gpu.memoryDump();

    my_driver.vGpuMemcpy(cpu_data_1.data(), bytes_package_1, MemcpyKind::HostToDevice);
    my_gpu.memoryDump();
    my_driver.vGpuMemcpy(cpu_data_2.data(), bytes_package_2, MemcpyKind::HostToDevice);
    my_gpu.memoryDump();

    std::fill(cpu_data_1.begin(), cpu_data_1.end(), 0);
    std::cout << "CPU data cleared to 0. Preparing to fetch from GPU... \n";

    my_driver.vGpuMemcpy(cpu_data_1.data(), bytes_package_1, MemcpyKind::DeviceToHost);

    std::cout << "Data recovered from vGPU: \n";
    for (int n : cpu_data_1) std::cout << n << " ";
    std::cout << "\n";

    my_gpu.memoryDump();
    my_gpu.printStatus();

    std::cout<<"SIM Stopped\n";

    return 0;
}