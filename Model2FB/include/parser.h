#ifndef _ModelParser_
#define _ModelParser_

#include <iostream>
#include <fstream>
#include <memory>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <nlohmann/json.hpp>
#include <flatbuffers/flatbuffers.h>
#include <model_generated.h>

using namespace std;
using json = nlohmann::json;

class ModelParser {
private:
    std::unique_ptr<tflite::FlatBufferModel> tf_flat_model; 
    const tflite::Model* tf_model;
    const tflite::SubGraph* getTFModelBackBone(const tflite::Model *tf_model);
    flatbuffers::Offset<my_model::Operator> parseConvLayer(
        const tflite::SubGraph* tf_backbone, const tflite::Operator* op, const size_t op_index,
        flatbuffers::FlatBufferBuilder& builder, const string& conv_op
    );

public:
    ModelParser(const char* model_path) {
        tf_flat_model = tflite::FlatBufferModel::BuildFromFile(model_path);
    }
    ~ModelParser() {}

    void setTFModel(const char* model_path);
    void parseBackBoneToFlatBuffer(const char* output_file_path, const char* model_name);
};


#endif
