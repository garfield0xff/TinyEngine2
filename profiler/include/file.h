#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "profiler.h"

using namespace std;

class File : public profiler{
private:
    

public:
    string file_path = "/Users/gyujinkim/Desktop/Ai/TinyEngine2/profiler/TEST/TEST.csv";
    string get_Path();
    void set_Path();
    bool isFileEmpty(const string& filename);
    void write_Data(int index, string& model_name, string& client_name, double accurancy, double duration, float cpu_usage, long rss);
    std::vector<std::vector<std::string>> readCSV(const std::string &filePath);
    void clearCSVFile(const std::string& filePath);
    void printCSV();
    void printRowWithHeader(int index);
    void deleteRowAndReindex(int indexToDelete);

};