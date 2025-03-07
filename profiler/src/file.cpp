#include "file.h"

// CSV 파일 경로 얻기
string File::get_Path(){ 
    return file_path;
}

// CSV 파일 경로 지정하기
void File::set_Path(){
    this->file_path = "/Users/gyujinkim/Desktop/Ai/TinyEngine2/profiler/TEST/TEST.csv";
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
void File::write_Data(int index, string& model_name, string& client_name, double accurancy, double duration, float cpu_usage, long rss){
    std::ofstream out(File::file_path, ios::app);
    bool isFirstRun = File::isFileEmpty(File::file_path);
    if(!out.is_open()){
        std::cerr << "파일을 열 수 없습니다: " << File::file_path << std::endl;
    }
    else{
        if(isFirstRun){
            out << "Index,Model Name,Client Name,Accurancy,Total Time(ms),CPU Usage(%),Memory Usage(KB)" << std::endl;
        }
        out << index << "," << model_name << "," << client_name << ","
            << accurancy << "," << duration << ","  << cpu_usage << "," 
            << rss << std::endl;
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

void File::printCSV(){
    std::ofstream out(File::file_path, ios::app);
    bool isFirstRun = File::isFileEmpty(File::file_path);
    if(isFirstRun){
        out << "Index,Model Name,Client Name,Accurancy,Total Time(ms),CPU Usage(%),Memory Usage(KB)" << std::endl;
    }

    const int columnWidth = 20; // 열 너비를 설정
        bool isFirstLine = true;

    for (const auto &row : this->readCSV(this->get_Path())){
        if (isFirstLine) {
            std::cout << "\033[30;47m"; // 검은 텍스트, 흰 배경
            for (const auto& cell : row) {
                std::cout << setw(columnWidth) << left << cell;
            }
            std::cout << "\033[0m" << std::endl; // 스타일 초기화
            isFirstLine = false; // 이후 줄은 스타일 없이 출력
        }
        else{
            for (const auto &cell : row){
                std::cout << setw(columnWidth) << left << cell; // 열 너비를 설정하고 왼쪽 정렬
            }
            cout << endl;
        }
    }
}

// 특정 index의 행과 헤더를 스타일을 적용하여 출력하는 메소드
void File::printRowWithHeader(int index) {
    const int columnWidth = 20; // 열 너비를 설정
    bool isFirstLine = true;

    // CSV 파일 전체 읽기
    std::vector<std::vector<std::string>> data = File::readCSV(File::file_path);

    if (data.empty()) {
        std::cerr << "CSV 파일이 비어 있거나 읽을 수 없습니다." << std::endl;
        return;
    }

    // 헤더 및 데이터 출력
    for (size_t rowIndex = 0; rowIndex < data.size(); ++rowIndex) {
        const auto& row = data[rowIndex];

        if (isFirstLine) {
            std::cout << "\033[30;47m"; // 검은 텍스트, 흰 배경
            for (const auto& cell : row) {
                std::cout << std::setw(columnWidth) << std::left << cell;
            }
            std::cout << "\033[0m" << std::endl; // 스타일 초기화
            isFirstLine = false; // 이후 줄은 스타일 없이 출력
        } else if (static_cast<int>(rowIndex) == index) {
            for (const auto& cell : row) {
                std::cout << std::setw(columnWidth) << std::left << cell;
            }
            std::cout << std::endl;
        }
    }
}

void File::deleteRowAndReindex(int indexToDelete) {
    // CSV 파일 전체 읽기
    std::vector<std::vector<std::string>> data = readCSV(file_path);

    if (data.empty()) {
        std::cerr << "CSV 파일이 비어 있거나 읽을 수 없습니다." << std::endl;
        return;
    }

    // 첫 번째 줄(헤더) 제외한 데이터에서 인덱스가 일치하는 행 삭제
    if (indexToDelete < 1 || indexToDelete >= data.size()) {
        std::cerr << "유효하지 않은 인덱스입니다." << std::endl;
        return;
    }

    // 인덱스에 해당하는 행 삭제
    data.erase(data.begin() + indexToDelete);

    // 인덱스 열을 새로 조정
    for (size_t i = 1; i < data.size(); ++i) {
        data[i][0] = std::to_string(i); // 첫 번째 열(인덱스) 재정렬
    }

    // 수정된 내용을 파일에 다시 저장
    std::ofstream outFile(file_path, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << file_path << std::endl;
        return;
    }

    // 나머지 데이터 작성
    for (size_t rowIdx = 0; rowIdx < data.size(); ++rowIdx) {
        const auto& row = data[rowIdx];
        for (size_t colIdx = 0; colIdx < row.size(); ++colIdx) {
            outFile << row[colIdx];
            if (colIdx < row.size() - 1) {
                outFile << ","; // 열 구분자만 추가
            }
        }
        outFile << std::endl;
    }

    outFile.close();
}


