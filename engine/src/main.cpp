#include "parser.h"

#include <iostream>


using namespace std;

int main() {
    FBParser fb_p1;
    const char* binary_path = "/Users/gyujinkim/Desktop/Ai/TinyEngine2/Model2FB/backbone.bin";

    fb_p1.loadFBFromFile(binary_path);
}
