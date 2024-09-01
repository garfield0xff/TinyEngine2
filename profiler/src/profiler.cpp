#include "profiler.h"
#include "memory.h"
#include "cpu.h"
#include "file.h"

// 현재의 인덱스 출력 및 반환
int profiler::cnt_Index(){
    File f;
    index = read_LastIndexFromFirstColumn(f.get_Path());
    index++;
    cout << "Index : " << index << endl;
    return index;
}

// CSV파일에서 존재하는 마지막 인덱스
int profiler::read_LastIndexFromFirstColumn(string filename){
    ifstream file(filename);
    if (!file.is_open()){
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return -1;
    }
    string line;
    int lastindex = 0;
    // 첫 번째 줄은 건너뜀. (소제목 부분이므로)
    getline(file, line);
    while (getline(file, line)){
        istringstream stream(line);
        string firstColumnValue;
        if (getline(stream, firstColumnValue, ',')){
            lastindex = stoi(firstColumnValue); }
    }

    index = lastindex;
    file.close();
    return lastindex;
}

// 알고리즘 시작시간
void profiler::start_Time(){
    profiler::start_time = chrono::high_resolution_clock::now();
}

// 알고리즘 종료시간
void profiler::stop_Time(){
    profiler::stop_time = chrono::high_resolution_clock::now();
    profiler::duration_time = stop_time - start_time;
    cout << "Execution time: " << duration_time.count() << " ms" << endl;
}

// 알고리즘 총 소요시간
double profiler::duration_Time(){
    return duration_time.count();
}

// 메뉴
void profiler::displayMenu(profiler *p){
    int ch;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "             Profiler Manager                " << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "  1. Display Profiler List                   " << endl;
    cout << "  2. Input Profiler                          " << endl;
    cout << "  3. Delete Profiler                         " << endl;
    cout << "  4. Find Index Row                          " << endl;
    cout << "  5. Quit This Program                       " << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << " What do you want to do? ";
    cin >> ch;
    cout << endl;

    CPU c;
    Memory m;
    File f;

    // 종료
    if (ch == 5)
    {
        _exit(0);
    }

    // 알고리즘 실행하기
    else if (ch == 2)
    {
        {
            p->flag = true;

            // c.startMacRuntimeThread();

            m.printLinuxMemoryUsage(p->flag);
            c.startLinuxRuntimeThread();
            p->start_Time();
        }

         cg1->parseTFModel(tf_model);

        {
            p->stop_Time();
            p->flag = false;

            // c.endMacRuntimeThread();

            m.printLinuxMemoryUsage(p->flag);
            c.endLinuxRuntimeThread();
        }

        // 측정 내용 CSV에 작성하기
        f.write_Data(cnt_Index(), duration_Time(), 
                    c.returnLinuxCpuUsageMemory(),
                    m.calLinuxMemorydiff(), f.get_Path());
    }

    // CSV파일 전체 내용 출력
    else if (ch == 1)
    {
        const int columnWidth = 30; // 열 너비를 설정
        for (const auto &row : f.readCSV(f.get_Path())){
            for (const auto &cell : row){
                cout << setw(columnWidth) << left << cell; // 열 너비를 설정하고 왼쪽 정렬
            }
            cout << endl;
        }
    }

    // CSV 삭제 메뉴
    else if (ch == 3)
    {
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "             Profiler Manager                " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Delete All                              " << endl;
        cout << "  2. Select a row to delete                  " << endl;
        cout << "  3. Back to Menu                            " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " What do you want to do? ";
        int ch4_Opt;
        cin >> ch4_Opt;
        if(ch4_Opt == 1){
            f.clearCSVFile(f.get_Path());
        }
        else if(ch4_Opt==2){
            int ch_Row;
            cout << "Select a row" << endl;
            cin >> ch_Row;
        }
    }
    cout << endl;
}
