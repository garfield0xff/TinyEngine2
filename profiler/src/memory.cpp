#include "memory.h"

#ifdef __APPLE__
void Memory::printMemoryUsage() {
    mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        std::cerr << "Failed to get memory usage information." << std::endl;
        return;
    }

    // Resident Set Size (RSS) in bytes
    rss = "VmRSS: " + std::to_string(info.resident_size / 1024) + " kB";
    std::cout << rss << std::endl;
}

#elif __linux__

// rss[0]은 시작 전 메모리 크기, rss[1]은 종료 후 메모리 크기
long Memory::rss[2] = {0};

// 시스템 파일에서 자기 프로세스의 실제 메모리 사용량 확인하기
void Memory::printLinuxMemoryUsage(bool flag){       
    ifstream statusFile("/proc/self/status");
    if (!statusFile.is_open()) {
        return;
    }

    string line;
    while (getline(statusFile, line)) {
        if (line.find("VmRSS") == 0) {
            size_t pos = line.find_first_of("0123456789");
            if(flag == true){
                Memory::rss[0] = std::stol(line.substr(pos));
                std::cout << "Before " << line << std::endl;
            }
            else{
                Memory::rss[1] = std::stol(line.substr(pos));
                std::cout << "After " << line << std::endl;
            }
            break;
        }
    }
}

// 실행 전과 후, 총 메모리 차이 반환하기
long Memory::calLinuxMemorydiff(){
    return  (Memory::rss[1] - Memory::rss[0]) ; 
}

#endif