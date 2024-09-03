#ifndef _FBParser_
#define _FBParser_

#include "model_generated.h"

#include <fstream>
#include <iostream>

using namespace std;

class FBParser {
    public:
        void printSampleFBFromFile(const char *filename);
        void loadFBFromFile(const char *filename);
};

#endif