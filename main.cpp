#include <iostream>
#include <vector>
#include <cstring>
#include "device.hpp"
#include "driver.hpp"

int main() {
    std::cout<<"vGPU Started v0.2"<<std::endl;

    DeviceDescriptor intel_gpu_config;
    intel_gpu_config.device_name = "Skylake Graphics (Gen9)";
    intel_gpu_config.vendor_name = "Intel";
    intel_gpu_config.litography = 14; 
    intel_gpu_config.memory_size = 1024 * 32;
    intel_gpu_config.architecture = "super_arch1";
    intel_gpu_config.core_clock_mhz = 3200;
    intel_gpu_config.execution_units = 4;

    Device my_gpu(intel_gpu_config);
    Driver my_driver(&my_gpu);

    std::vector<int> cpu_data = {10, 20, 30, 40, 50};
    size_t bytes_in_package = cpu_data.size() * sizeof(int);

    bool isRunning = true;
    std::string command;

    while (isRunning) {
        std::cout << "<\\>";
        std::cin >> command;

        if (command == "status") {
            my_gpu.printStatus();
        }
        else if (command == "dump") {
            my_gpu.memoryDump();
        }
        else if (command == "clear") {
            my_gpu.resetVram();
            std::cout << "VRAM reinitialized \n";
        }
        else if (command == "upload") {
            my_driver.vGpuMemcpy(cpu_data.data(), bytes_in_package, MemcpyKind::HostToDevice);
        }
        else if (command == "download") {
            my_driver.vGpuMemcpy(cpu_data.data(), bytes_in_package, MemcpyKind::DeviceToHost);

            std::cout << "Data recovered from vGPU: \n";
            for (int n : cpu_data) std::cout << n << " ";
            std::cout << "\n";
        }
        else if (command == "exit" || command == "quit") {
            std::cout << "Leaving simulation...\n";
            isRunning = false;
        }
        else if (command == "change") {
            std::cout << "Please type 5 numbers separated by a space: ";
            std::cin >> cpu_data[0] >> cpu_data[1] >> cpu_data[2] >> cpu_data[3] >> cpu_data[4];
        }
        else if ( command == "compute") {
            auto krnl_multiply_by_100 = [](unsigned char* vram, size_t active_bytes) {
                int* int_data = reinterpret_cast<int*>(vram);
                size_t num_elements = active_bytes / sizeof(int);

                for (size_t i = 0; i < num_elements; ++i) {
                    int_data[i] = int_data[i] * 100;
                }
            };

            // auto krnl_modulo = [](unsigned char* vram, size_t active_bytes) {
            //     int* int_data = reinterpret_cast<int*>(vram);
            //     size_t num_elements = active_bytes / sizeof(int);

            //     for (size_t i = 0; i < num_elements; ++i) {
            //         if (int_data[i]%2 == 0) {
            //             int_data[i] = 2;
            //         }
            //         else {
            //             int_data[i] = 1;
            //         }
            //     }
            // };
            auto krnl_modulo = [](int* data, size_t num_elements) {
                for (size_t i = 0; i < num_elements; ++i) {
                    if (data[i] % 2 == 0) data[i] = 2;
                    else data[i] = 1;
                }
            };

            my_gpu.executeKernel(krnl_modulo);
        }
        else {
            std::cout << "Unknown command. Please use: status, dump, clear, upload, download, compute, exit\n";
        }

    }

    std::cout<<"SIM Stopped\n";

    return 0;
}