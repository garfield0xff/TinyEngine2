#include "parser.h"
#include "test_parser.h"
#include "model_generated.h"
#include <flatbuffers/flatbuffers.h>

int main()
{
    TestCaseParser tp1;
    // saveBinaryFlatBuffer("model.bin");
    tp1.loadBinaryFlatBuffer("backbone.bin");


    #if 0
    const char* model_path = "model.tflite";
    const char* output_file_path = "backbone.bin";
    ModelParser* mp1 = new ModelParser(model_path);
    mp1->setTFModel(model_path);
    mp1->parseBackBoneToFlatBuffer(output_file_path, "person_detection");
    #endif

    return 0;
}