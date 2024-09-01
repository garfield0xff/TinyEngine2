#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class File{
private:
    

public:
    string file_path = "TEST/TEST.csv";
    string get_Path();
    void set_Path();
    bool isFileEmpty(const string& filename);
    void write_Data(int index, double duration, float CPU_Usage, long VmRSS, const string& filename);
    std::vector<std::vector<std::string>> readCSV(const std::string &filePath);
    void clearCSVFile(const std::string& filePath);
    

};