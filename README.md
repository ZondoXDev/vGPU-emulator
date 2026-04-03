# 🚀 vGPU-Emulator

A user-space, C++ based simulator of a discrete Graphics Processing Unit (GPU) and heterogeneous computing environment. 

This project is built from scratch to deeply explore and understand low-level systems programming, including memory management, Host-to-Device communication (simulated PCIe bus), and multithreaded task execution, mirroring the architectural concepts used in modern GPU APIs (like SYCL, CUDA or Vulkan).

## 🧠 Core Architecture
The simulator separates the processing environment into distinct components, adhering to the heterogeneous computing paradigm:
* **Host (CPU):** Represents the main system memory and application logic.
* **Device (GPU):** A configurable virtual hardware unit with its own simulated VRAM (dynamically allocated on the heap) and operational states.
* **Driver:** The bridge API responsible for managing memory transfers (`HostToDevice` / `DeviceToHost`) safely, preventing memory leaks and segmentation faults.

## ⚙️ Features & Roadmap

- [x] **Hardware Abstraction:** Configurable GPU architecture via Descriptor patterns (Vendor, Execution Units, Clock Speed, VRAM size).
- [x] **Safe Memory Management:** RAII implementation for VRAM allocation and cleanup.
- [ ] **PCIe Bus Simulation:** Raw byte transfers using `std::memcpy` with strict VRAM capacity validation (OOM prevention). *[In Progress]*
- [ ] **Command Queue & Execution:** Dispatching compute kernels (C++ lambdas) to the Device.
- [ ] **Parallel Processing:** Utilizing `std::thread` to simulate multiple Execution Units working simultaneously on VRAM chunks.
- [ ] **Modern C++ Refactor:** Transitioning to `std::unique_ptr` and polymorphic GPU generations.

## 🛠️ Tech Stack
* **Language:** Modern C++17
* **Build System:** CMake
* **Environment:** Linux (User-space application)

## 🚀 Build & Run (Linux)
Ensure you have a C++17 compatible compiler and CMake installed (`build-essential` and `cmake`).

```bash
# Clone the repository
git clone https://github.com/ZondoXDev/vGPU-emulator.git
cd vGPU-emulator

# Create a build directory
mkdir build && cd build

# Configure and compile
cmake ..
make

# Run the simulator
./vgpu_sim
