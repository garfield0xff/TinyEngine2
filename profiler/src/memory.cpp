#include "memory.h"

#ifdef __APPLE__

long Memory::rss[2] = {0};

void Memory::printMacMemoryUsage(bool flag) {
#ifdef __APPLE__
void Memory::printMemoryUsage() {
    mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        std::cerr << "Failed to get memory usage information." << std::endl;
        return;
    }

    if(flag == true){
        Memory::rss[0] = info.resident_size / 1024; // resident_size is in bytes, convert to kilobytes
        std::cout << "Before RSS: " << Memory::rss[0] << " kB" << std::endl;
    }
    else{
        Memory::rss[1] = info.resident_size / 1024; // resident_size is in bytes, convert to kilobytes
        std::cout << "After RSS: " << Memory::rss[1] << " kB" << std::endl;
    }
    // Resident Set Size (RSS) in bytes
    rss = "VmRSS: " + std::to_string(info.resident_size / 1024) + " kB";
    std::cout << rss << std::endl;
}

#elif __linux__

long Memory::rss[2] = {0};

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
#endif


long Memory::calMemorydiff(){
    return  (Memory::rss[1] - Memory::rss[0]) ; 
}


long Memory::calLinuxMemorydiff(){
    return  (Memory::rss[1] - Memory::rss[0]) ; 
}

#endif

