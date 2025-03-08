## Project
This project is designed to parse AI Model(tensorflow, torch) using tvm library and optimize IR Level

This project was created with reference to TinyEngine by MIT HAN Lab 
[Link](https://github.com/mit-han-lab/tinyengine)

## PIPELINE
<img width="800" alt="Image" src="https://github.com/user-attachments/assets/3e1dd5d1-3e23-4dfa-b87f-13308f78f833" />

## TEAM
- 김규진 (Kim Gyu Jin) [Github](https://github.com/garfield0xff) email : garfield0xff@gmail.com
- 김한빈 (Kim Han bin) [Github](https://github.com/hanbeen00)
email : hanbini2008@naver.com

## Installation

### Package  (Required)

1. [TVM](https://github.com/apache/tvm)
```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DUSE_LLVM=ON \
  -DUSE_CUDA=OFF \   // If you use GPU, -DUSE_CUDA=ON
  -DUSE_OPENCL=OFF \
  -DUSE_VULKAN=OFF \
  -DUSE_METAL=OFF
```
2. [Flatbuffers](https://flatbuffers.dev) 4.8.0


#### Edge Device (Raspberry5)
- Processor: Broadcom BCM2712 
- GPU: VideoCore VII 
- CPU Clock Speed: Quad-core @ 2.4GHz 
- RAM: 4GB LPDDR4X-4267 SDRAM 
- Maximum Power Consumption / Voltage: 5A @ 5V 


 
