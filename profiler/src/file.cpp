#include "file.h"
#include "File.h"

#ifdef __APPLE__


#elif __linux__


string File::get_Path(){ 
    return file_path;
}

void File::set_Path(){
    this->file_path = "TEST.csv";
    file_path = "TEST/TEST.csv"; 
}


bool File::isFileEmpty(const string& filename) { 
    std::ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        return true;
    }
    return file.peek() == ifstream::traits_type::eof();
}


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

    const int columnWidth = 20; 
        bool isFirstLine = true;

    for (const auto &row : this->readCSV(this->get_Path())){
        if (isFirstLine) {
            std::cout << "\033[30;47m"; 
            for (const auto& cell : row) {
                std::cout << setw(columnWidth) << left << cell;
            }
            std::cout << "\033[0m" << std::endl; 
            isFirstLine = false; 
        }
        else{
            for (const auto &cell : row){
                std::cout << setw(columnWidth) << left << cell; 
            }
            cout << endl;
        }
    }
}

void File::printRowWithHeader(int index) {
    const int columnWidth = 20; 
    bool isFirstLine = true;

    std::vector<std::vector<std::string>> data = File::readCSV(File::file_path);

    if (data.empty()) {
        std::cerr << "cannot read csv file." << std::endl;
        return;
    }


    for (size_t rowIndex = 0; rowIndex < data.size(); ++rowIndex) {
        const auto& row = data[rowIndex];

        if (isFirstLine) {
            std::cout << "\033[30;47m"; 
            for (const auto& cell : row) {
                std::cout << std::setw(columnWidth) << std::left << cell;
            }
            std::cout << "\033[0m" << std::endl; 
            isFirstLine = false; 
        } else if (static_cast<int>(rowIndex) == index) {
            for (const auto& cell : row) {
                std::cout << std::setw(columnWidth) << std::left << cell;
            }
            std::cout << std::endl;
        }
    }
}

void File::deleteRowAndReindex(int indexToDelete) {
    std::vector<std::vector<std::string>> data = readCSV(file_path);

    if (data.empty()) {
        std::cerr << "cannot read csv file." << std::endl;
        return;
    }


    if (indexToDelete < 1 || indexToDelete >= data.size()) {
        std::cerr << "Invalid index." << std::endl;
        return;
    }

    data.erase(data.begin() + indexToDelete);

    for (size_t i = 1; i < data.size(); ++i) {
        data[i][0] = std::to_string(i); 
    }

    std::ofstream outFile(file_path, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "cannot open file " << file_path << std::endl;
        return;
    }

    for (size_t rowIdx = 0; rowIdx < data.size(); ++rowIdx) {
        const auto& row = data[rowIdx];
        for (size_t colIdx = 0; colIdx < row.size(); ++colIdx) {
            outFile << row[colIdx];
            if (colIdx < row.size() - 1) {
                outFile << ","; 
            }
        }
        outFile << std::endl;
    }

    outFile.close();
}


#endif

