#include "memory.h"

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