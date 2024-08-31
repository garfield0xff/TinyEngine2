#include "parser.h"


void ModelParser::setTFModel(const char* model_path) {
    std::cout << "setTFModel Func()" << std::endl;

    if (!tf_flat_model) {
        std::cerr << "Failed to load model" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    tf_model = tf_flat_model->GetModel();

    if (!tf_model) {
        std::cerr << "Failed to get model from FlatBufferModel" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}



const tflite::SubGraph* ModelParser::getTFModelBackBone(const tflite::Model *tf_model)
{
    cout << "getTFModelBackbone Func()" << endl;
    const tflite::SubGraph* subgraph = tf_model->subgraphs()->Get(0);
    if (!subgraph) {
        std::cerr << "Failed to load subgraph" << std::endl;
        return nullptr;
    }
    return subgraph;
}


flatbuffers::Offset<my_model::Operator> ModelParser::parseConvLayer(
    const tflite::SubGraph* tf_backbone , const tflite::Operator *op, const size_t op_index,
    flatbuffers::FlatBufferBuilder &builder,  flatbuffers::Offset<my_model::Operator> fb_op_offset,
    const char* conv_op ) 
{

    auto filter_tensor = tf_backbone->tensors()->Get(op->inputs()->Get(1));
    auto filter_shape = filter_tensor->shape();
    auto filter_buffer = tf_model->buffers()->Get(filter_tensor->buffer());
    const float* filter_data = reinterpret_cast<const float*>(filter_buffer->data()->data());
    size_t filter_buffer_size = filter_buffer->data()->size() / sizeof(float);
    auto weight_tensor = my_model::CreateTensor(
        builder,
        builder.CreateVector(std::vector<int32_t>(filter_shape->begin(), filter_shape->end())),
        builder.CreateVector(filter_data, filter_buffer_size)
    );

    auto bias_tensor = tf_backbone->tensors()->Get(op->inputs()->Get(2));
    auto bias_shape = bias_tensor->shape();
    auto bias_buffer = tf_model->buffers()->Get(bias_tensor->buffer());
    const float* bias_data = reinterpret_cast<const float*>(bias_buffer->data()->data());
    size_t bias_buffer_size = bias_buffer->data()->size() / sizeof(float);
    auto fb_bias_tensor_offset = my_model::CreateTensor(
        builder,
        builder.CreateVector(std::vector<int32_t>(bias_shape->begin(), bias_shape->end())),
        builder.CreateVector(bias_data, bias_buffer_size)
    );
         
    auto conv_options = op->builtin_options_as_Conv2DOptions();
    string act_func = (conv_options->fused_activation_function() == tflite::ActivationFunctionType_RELU6) ? "RELU6" : "NULL";
    string pad = (conv_options->padding() == tflite::Padding_VALID) ? "VALID" : "SAME";
    auto fb_conv_options = my_model::CreateConv2DOptions(
        builder,
        builder.CreateString(act_func),
        builder.CreateString(pad),
        builder.CreateVector(std::vector<int32_t>({1, 1}))
    );
    auto fb_conv_options_vector = builder.CreateVector(
        std::vector<flatbuffers::Offset<my_model::Conv2DOptions>>{fb_conv_options}
    );

    auto output_tensor = tf_backbone->tensors()->Get(op->outputs()->Get(0));
    auto output_shape = output_tensor->shape();
    return my_model::CreateOperator(
        builder,
        op_index,
        builder.CreateString("Conv2D"),
        weight_tensor,
        fb_bias_tensor_offset,
        builder.CreateVector(std::vector<int32_t>({
            output_shape->Get(1),
            output_shape->Get(2),
            output_shape->Get(3)
        })),
        fb_conv_options_vector
    );
}


void ModelParser::parseBackBoneToFlatBuffer(const char* output_file_path, const char* model_name) {
    std::cout << "parseBackBoneToFlatBuffer Func()" << std::endl;

    const tflite::SubGraph* tf_backbone = getTFModelBackBone(tf_model);

    if (!tf_backbone) {
        std::cerr << "Error: getTFModelBackBone()" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    flatbuffers::FlatBufferBuilder builder(1024);  

    std::vector<flatbuffers::Offset<my_model::Operator>> fb_operators_vector;
    
    for (size_t i = 0; i < tf_backbone->operators()->size(); ++i) {
        auto op = tf_backbone->operators()->Get(i);
        auto opcode_index = op->opcode_index();
        auto opcode = tf_model->operator_codes()->Get(opcode_index)->builtin_code();

        flatbuffers::Offset<my_model::Operator> fb_op_offset;

        switch (opcode) {
        case tflite::BuiltinOperator_CONV_2D:
        {
            fb_op_offset = parseConvLayer(tf_backbone, op, i, builder, fb_op_offset, "Conv2D");
            break;
        }
        case tflite::BuiltinOperator_DEPTHWISE_CONV_2D:
        {
            
        }

        default:
            continue;  
        }
        
        fb_operators_vector.push_back(fb_op_offset);
    }

    auto subgraph = my_model::CreateSubGraph(
        builder,
        0, 
        builder.CreateVector(fb_operators_vector)
    );

    auto fb_subgraphs = builder.CreateVector(std::vector<flatbuffers::Offset<my_model::SubGraph>>{subgraph});
    auto model = my_model::CreateModel(
        builder,
        builder.CreateString(model_name),
        fb_subgraphs
    );

    builder.Finish(model);

    std::ofstream out_file(output_file_path, std::ios::binary);
    if (out_file.is_open()) {
        out_file.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), builder.GetSize());
        out_file.close();
        std::cout << "Parse BackBone to FlatBuffer Successfully" << std::endl;
    } else {
        std::cerr << "Failed to open output file: " << output_file_path << std::endl;
    }
}
