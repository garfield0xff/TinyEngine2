#include "cpu.h"

#ifdef __APPLE__
void* CPU::getMacCpuUsageMemory(void *arg) {
    CPU* cpu_instance = static_cast<CPU*>(arg); 

    Jiffies curJiffies = {};
    Jiffies prevJiffies = {};

    // 처음 prevJiffies를 초기화하기 위해 macOS에서 CPU 사용량 정보를 읽습니다.
    mach_msg_type_number_t count;
    host_cpu_load_info_data_t cpu_info;
    kern_return_t error;

    count = HOST_CPU_LOAD_INFO_COUNT;
    error = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpu_info, &count);
    if (error != KERN_SUCCESS) {
        std::cerr << "Failed to get CPU info" << std::endl;
        return NULL;
    }

    prevJiffies.user = cpu_info.cpu_ticks[CPU_STATE_USER];
    prevJiffies.nice = cpu_info.cpu_ticks[CPU_STATE_NICE];
    prevJiffies.system = cpu_info.cpu_ticks[CPU_STATE_SYSTEM];
    prevJiffies.idle = cpu_info.cpu_ticks[CPU_STATE_IDLE];

    while (cpu_instance->th_is_running) {  // 스레드 실행 상태 확인
        error = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpu_info, &count);
        if (error != KERN_SUCCESS) {
            std::cerr << "Failed to get CPU info" << std::endl;
            usleep(300000); 
            continue;
        }

        curJiffies.user = cpu_info.cpu_ticks[CPU_STATE_USER];
        curJiffies.nice = cpu_info.cpu_ticks[CPU_STATE_NICE];
        curJiffies.system = cpu_info.cpu_ticks[CPU_STATE_SYSTEM];
        curJiffies.idle = cpu_info.cpu_ticks[CPU_STATE_IDLE];

        Jiffies diffJiffies;
        diffJiffies.user = curJiffies.user - prevJiffies.user;
        diffJiffies.nice = curJiffies.nice - prevJiffies.nice;
        diffJiffies.system = curJiffies.system - prevJiffies.system;
        diffJiffies.idle = curJiffies.idle - prevJiffies.idle;

        int totalJiffies = diffJiffies.user + diffJiffies.nice + diffJiffies.system + diffJiffies.idle;

        if (totalJiffies > 0) {
            cpu_instance->cpu_usage = 100.0f * (1.0 - (diffJiffies.idle / (double) totalJiffies));
            if(cpu_instance->cpu_max < cpu_instance->cpu_usage){
                cpu_instance->cpu_max = cpu_instance->cpu_usage;
            }
        } else {
            cpu_instance->cpu_usage = 0.0f;  // default 값을 설정하거나 에러를 처리
        }

        prevJiffies = curJiffies;

        usleep(50000); // 루프 반복을 늦추기 위해 잠시 대기
    }
    return NULL;
}


void CPU::startMacRuntimeThread() { 
    th_is_running = true;  // 스레드가 실행 중임을 표시
    int res = pthread_create(&cpu_thread, NULL, getMacCpuUsageMemory, this);
    if (res != 0) {
        std::cerr << "Failed to create thread: " << res << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void CPU::endMacRuntimeThread() {
    if (th_is_running) {
        th_is_running = false;  // 스레드 종료 신호를 보냄
        pthread_join(cpu_thread, NULL);  // 스레드가 종료될 때까지 대기
        printf("Cpu usage : %.2f%%\n", cpu_max);
        std::cout << "CPU monitoring thread has been terminated." << std::endl;
    }
}

float CPU::returnMacCpuUsageMemory(){
    return cpu_usage;
}


#elif __linux__

// 리눅스 시스템 파일에서 CPU 정보 찾기
void* CPU::getLinuxCpuUsageMemory(void *arg) {      
    CPU* cpu_instance = static_cast<CPU*>(arg); 
    FILE *pStat = NULL;
    char cpuID[6] = {0};

    Jiffies curJiffies = {};
    Jiffies prevJiffies = {};

    // 처음 prevJiffies를 초기화하기 위해 /proc/stat를 읽습니다.
    pStat = fopen("/proc/stat", "r");
    if (pStat != NULL) {
        fscanf(pStat, "%s %d %d %d %d", cpuID, &prevJiffies.user,
               &prevJiffies.nice, &prevJiffies.system, &prevJiffies.idle);
        fclose(pStat);
    }

    while (cpu_instance->th_is_running) {  // 스레드 실행 상태 확인
        pStat = fopen("/proc/stat", "r");
        if (pStat == NULL) {
            std::cerr << "Failed to open /proc/stat" << std::endl;
            usleep(300000); 
            continue;
        }

        fscanf(pStat, "%s %d %d %d %d", cpuID, &curJiffies.user,
               &curJiffies.nice, &curJiffies.system, &curJiffies.idle);

        Jiffies diffJiffies;
        diffJiffies.user = curJiffies.user - prevJiffies.user;
        diffJiffies.nice = curJiffies.nice - prevJiffies.nice;
        diffJiffies.system = curJiffies.system - prevJiffies.system;
        diffJiffies.idle = curJiffies.idle - prevJiffies.idle;

        int totalJiffies = diffJiffies.user + diffJiffies.nice + diffJiffies.system + diffJiffies.idle;
        
         if (totalJiffies > 0) {
            cpu_instance->cpu_usage = 100.0f * (1.0 - (diffJiffies.idle / (double) totalJiffies));
            if(cpu_instance->cpu_max < cpu_instance->cpu_usage){ // CPU 최대값 구하기
                cpu_instance->cpu_max = cpu_instance->cpu_usage;
            }
        } else {
            cpu_instance->cpu_usage = 0.0f;  // default 값을 설정하거나 에러를 처리
        }
        prevJiffies = curJiffies;
        fclose(pStat);
        usleep(50000);
    }
    return NULL;
}

// CPU 측정 스레드 실행시작
void CPU::startLinuxRuntimeThread() {
    th_is_running = true;  // 스레드가 실행 중임을 표시
    int res = pthread_create(&cpu_thread, NULL, getLinuxCpuUsageMemory, this);
    if (res != 0) {
        std::cerr << "Failed to create thread: " << res << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// CPU 측정 스레드 실행종료
void CPU::endLinuxRuntimeThread() { 
    if (th_is_running) {
        th_is_running = false;  // 스레드 종료 신호를 보냄
        pthread_join(cpu_thread, NULL);  // 스레드가 종료될 때까지 대기
        printf("Cpu usage : %.2f%%\n", cpu_max);
        std::cout << "CPU monitoring thread has been terminated." << std::endl;
    }
}

// 제일 최근 측정 마친 CPU값 리턴
float CPU::returnLinuxCpuUsageMemory() { 
    return cpu_usage;
}


#endif