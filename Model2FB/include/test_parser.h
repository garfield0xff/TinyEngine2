#ifndef _TEST_CASE_PARSER_
#define _TEST_CASE_PARSER_

#include "model_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <iostream>
#include <fstream>

using namespace std;


class TestCaseParser {
    public:
        void saveBinaryFlatBuffer(const char* filename);
        void loadBinaryFlatBuffer(const char* filename);
};


#endif