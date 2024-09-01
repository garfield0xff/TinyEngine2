#include "File.h"

#ifdef __APPLE__


#elif __linux__

// CSV 파일 경로 얻기
string File::get_Path(){ 
    return file_path;
}

// CSV 파일 경로 지정하기
void File::set_Path(){
    file_path = "TEST/TEST.csv"; // 디렉터리에 파일 실제로 있어야 실행됨
}

// CSV 파일 내부가 비었는지 확인
bool File::isFileEmpty(const string& filename) { 
    std::ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        return true;
    }
    return file.peek() == ifstream::traits_type::eof();
}

// 측정한 값 CSV 파일에 작성하기 (파일 내부가 비었으면 1행에 소제목 작성한 후 진행)
void File::write_Data(int index, double duration, float cpu_usage, long rss, const string& filename){
    std::ofstream out(File::file_path, ios::app);
    bool isFirstRun = File::isFileEmpty(File::file_path);
    if(!out.is_open()){
        std::cerr << "파일을 열 수 없습니다: " << File::file_path << std::endl;
    }
    else{
        if(isFirstRun){
            out << "Index, Total Time(ms), CPU Usage(%), Memory Usage(KB), File Path" << std::endl;
        }
        out << index << ", " << duration << ", " << cpu_usage << ","
            << rss << ", " << filename << std::endl;
    }
    out.close();
}

// CSV파일 전체 읽기
std::vector<std::vector<std::string>> File::readCSV(const std::string &filePath){
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filePath);
    if (!file.is_open()){
        std::cerr << "Error opening file: " << filePath << std::endl;
        return data;
    }
    std::string line;
    while (std::getline(file, line)){
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')){
            row.push_back(cell);
        }
        data.push_back(row);
    }
    file.close();
    return data;
}

// CSV파일 전체 내용 삭제
void File::clearCSVFile(const std::string& filePath){
    std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error opening file for clearing: " << filePath << std::endl;
            return;
        }
    file.close();
}
#endif

