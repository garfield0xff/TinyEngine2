#ifndef _CHECK_MEM_H_
#define _CHECK_MEM_H_

#pragma once

#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
    #include <mach/mach.h>
#elif __linux__
    #include "sys/sysinfo.h"
#endif



// using namespace std;

class Memory {
    private:
        std::string rss;
    
    public:
        void printMemoryUsage();
};


#endif //_CHECK_MEM_H_