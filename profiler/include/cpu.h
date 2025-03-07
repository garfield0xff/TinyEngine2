#ifndef _CHECK_CPUMEM_H_
#define _CHECK_CPUMEM_H_

#pragma once

#include <iostream>
#include <cstdio>
#include <sys/types.h>
// #include <sys/sysctl.h>
#include <pthread.h>
#include <unistd.h>

#ifdef __APPLE__
    #include <mach/mach.h>
    #include <mach/mach_host.h>

#elif __linux__
    #include <sys/types.h>
#endif

using namespace std;

class CPU {
    private:
        float cpu_usage;
        float cpu_max = 0;
        bool th_is_running;

        struct Jiffies
        {
            int user;
            int nice;
            int system;
            int idle;
        };

    public :
        pthread_t cpu_thread;

        #ifdef __APPLE__
            static void* getMacCpuUsageMemory(void *arg);
            void startMacRuntimeThread();
            void endMacRuntimeThread();
            float returnMacCpuUsageMemory();
        #elif __linux__
            static void* getLinuxCpuUsageMemory(void *arg);
            void startLinuxRuntimeThread();
            void endLinuxRuntimeThread();
            float returnLinuxCpuUsageMemory();
        #endif
        

};


#endif // _CHECK_CPUMEM_H_