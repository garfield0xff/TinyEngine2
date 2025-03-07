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
     cout << "Execution time: " << fixed << setprecision(4) << duration_time.count() << " ms" << endl;
}

// 알고리즘 총 소요시간
double profiler::duration_Time(){
    fps = 30/duration_time.count();
    return duration_time.count();
}

// 측정할 모델 이름 세팅 
void profiler::set_Modelname(){
    int ch;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "             Profiler Manager                " << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "  1. MobilenetV2                             " << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Select Model Name : ";
    cin >> ch;
    
    if(ch == 1){
        model_name = "MobilenetV2";
    }

    else{
        model_name = "";
        return;
    }
}

void profiler::write_Clientname(){
    std::cin >> client_name;
}

// 모델 정확도 얻기
void profiler::get_Accurancy(){
    accurancy = 0;
}


// 메뉴
void profiler::displayMenu(profiler *p){
    char ch;
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
    system("clear");

    CPU c;
    Memory m;
    File f;

    // CSV파일 전체 내용 출력
    if (ch == '1'){
        f.printCSV();
    }

    // 알고리즘 실행하기
    else if (ch == '2'){
        if(!p->flag){
            p->flag = true;
            set_Modelname();
            if(model_name == ""){
                return;
            }
            cout << "Write Client name : ";
            write_Clientname();
            if(client_name == ""){
                return;
            }
            m.printMacMemoryUsage(p->flag);
            // c.startMacRuntimeThread();
            c.startMacRuntimeThread();
            p->start_Time();
        }

        // cg1->parseTFModel(tf_model); //

        if(p->flag){
            p->stop_Time();
            p->flag = false;

            // c.endMacRuntimeThread();

            m.printMacMemoryUsage(p->flag);
            c.endMacRuntimeThread();
            
        }

        // 측정 내용 CSV에 작성하기
        f.write_Data(cnt_Index(), model_name, client_name, accurancy, duration_Time(), 
                    c.returnMacCpuUsageMemory(),
                    m.calMemorydiff());
    }

    // CSV 삭제 메뉴
    else if (ch == '3'){
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
        system("clear");

        if(ch4_Opt == 1){
            f.clearCSVFile(f.get_Path());
        }
        else if(ch4_Opt==2){
            f.printCSV();
            int ch_Row;
            cout << "Select a row : ";
            cin >> ch_Row;
            f.deleteRowAndReindex(ch_Row);
            system("clear");
            f.printCSV();
        }
        else{
            return;
        }
    }

    else if(ch=='4'){
        f.printCSV();
        int ch_Row;
        cout << "Select a row : ";
        cin >> ch_Row;
        system("clear");
        f.printRowWithHeader(ch_Row);
    }

    // 종료
    else if (ch == '5'){
        _exit(0);
    }

    else{
        return;
    }
    cout << endl;
}