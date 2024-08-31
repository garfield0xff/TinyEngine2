#ifndef _MEM_PROFILER_
#define _MEM_PROFILER_

#pragma once

#include <iostream>       
#include <chrono>         
#include <string>         
#include <atomic>         
#include <thread>         
#include <fstream>        
#include <sstream>        
#include <vector>         
#include <iomanip>        
#include <stdlib.h>       
#include <pthread.h>      
#include <unistd.h>       
#include <sys/types.h>    
#include <sys/sysctl.h>   

#ifdef __APPLE__
    #include <mach/mach.h>
#elif __linux__
    #include <sys/sysinfo.h>
#endif

using namespace std;

class profiler {
protected:
    chrono::high_resolution_clock::time_point start_time;
    chrono::high_resolution_clock::time_point stop_time;

    string image_path;
    string model_name;
    double prediction;
    bool flag = false;



public:
    int index = -1;
    int cnt_index();
    bool changedFlag();
    void displayMenu(profiler*);


public:
     void start_Time();
     void stop_Time();

public:
    //static void printMemoryUsage();


public:

};

#endif // _MEM_PROFILER_