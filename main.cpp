#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include "device.hpp"
#include "driver.hpp"

int main() {
    std::cout<<"vGPU Started v0.2"<<std::endl;

    DeviceDescriptor intel_gpu_config;
    intel_gpu_config.device_name = "Skylake Graphics (Gen9)";
    intel_gpu_config.vendor_name = "Intel";
    intel_gpu_config.litography = 14; 
    intel_gpu_config.memory_size = 1024 * 1024 * 4;
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

            auto krnl_modulo = [](int* data, size_t num_elements) {
                for (size_t i = 0; i < num_elements; ++i) {
                    if (data[i] % 2 == 0) data[i] = 2;
                    else data[i] = 1;
                }
            };

            my_gpu.executeKernel(krnl_modulo);
        }
        else if (command == "mandelbrot") {
            const int width = 160;
            const int height = 50;
            const int max_iter = 1000;
            size_t num_pixels = width * height;
            size_t img_bytes = num_pixels * sizeof(int);

            std::vector<int> cpu_img(num_pixels, 0); // empty canvas

            std::cout << "Uploading " << img_bytes << " bytes to VRAM...\n";
            my_driver.vGpuMemcpy(cpu_img.data(), img_bytes, MemcpyKind::HostToDevice);

            int* vram_global_start = reinterpret_cast<int*>(my_gpu.getVramStartPointer());

            auto krnl_mandelbrot = [vram_global_start, width, height, max_iter](int* local_data, size_t num_elements) {
                for (size_t i = 0; i < num_elements; ++i) {
                    size_t global_index = (local_data + i) - vram_global_start;
                    
                    int x = global_index % width;
                    int y = global_index / width;

                    float c_re = (x - width / 2.0f) * 4.0f / width;
                    float c_im = (y - height / 2.0f) * 4.0f / width;
                    
                    float z_re = 0.0f, z_im = 0.0f;
                    int iter = 0;

                    while (z_re * z_re + z_im * z_im <= 4.0f && iter < max_iter) {
                        float z_re_new = z_re * z_re - z_im * z_im + c_re;
                        z_im = 2.0f * z_re * z_im + c_im;
                        z_re = z_re_new;
                        iter++;
                    }

                    local_data[i] = iter;
                }
            };

            auto start_time = std::chrono::high_resolution_clock::now();
            
            my_gpu.executeKernel(krnl_mandelbrot);
            
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> compute_time = end_time - start_time;
            std::cout << "[PROFILER] Kernel execution took: " << compute_time.count() << " ms\n";

            my_driver.vGpuMemcpy(cpu_img.data(), img_bytes, MemcpyKind::DeviceToHost);

            std::cout << "\n--- Mandelbrot Set Render ---\n";
            const char* charset = " .:-=+*#%@"; // "color" palette
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int iters = cpu_img[y * width + x];
                    if (iters == max_iter) {
                        std::cout << " ";
                    } else {
                        int char_idx = (iters * 10) / 100;
                        if (char_idx > 9) char_idx = 9;
                        std::cout << charset[char_idx];
                    }
                }
                std::cout << "\n";
            }
            std::cout << "-----------------------------\n";
            
            my_gpu.resetVram();
        }
        else {
            std::cout << "Unknown command. Please use: status, dump, clear, upload, download, compute, exit\n";
        }

    }

    std::cout<<"SIM Stopped\n";

    return 0;
}