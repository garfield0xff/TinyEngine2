#include "test_parser.h"


void TestCaseParser::saveBinaryFlatBuffer(const char* filename) {

    flatbuffers::FlatBufferBuilder builder(1024);

    auto weight_shape = builder.CreateVector(std::vector<int32_t>{3, 3, 1, 16});
    std::vector<float> weight_data = {1.123, 2.234, 3.345, 4.456, 5.567};
    auto weight_values = builder.CreateVector(weight_data);
    auto weights = my_model::CreateTensor(builder, weight_shape, weight_values);

    auto bias_shape = builder.CreateVector(std::vector<int32_t>{16});
    std::vector<float> bias_data = {1, 2, 3, 4, 5};
    auto bias_values = builder.CreateVector(bias_data);
    auto bias = my_model::CreateTensor(builder, bias_shape, bias_values);

    auto activation_function = builder.CreateString("RELU");
    auto padding = builder.CreateString("SAME");
    auto strides = builder.CreateVector(std::vector<int32_t>{1, 1});

    auto conv_option = my_model::CreateConv2DOptions(
        builder,
        activation_function,
        padding,
        strides
    );

    auto conv_options = builder.CreateVector(std::vector<flatbuffers::Offset<my_model::Conv2DOptions>>{conv_option});
    
    auto outputs = builder.CreateVector(std::vector<int32_t>{1, 28, 28, 16});
    auto op = my_model::CreateOperator(
        builder,
        0, 
        builder.CreateString("Conv2D"),
        weights,
        bias,
        outputs,
        conv_options 
    );

    std::vector<flatbuffers::Offset<my_model::Operator>> operators_vector = {op};
    auto operators = builder.CreateVector(std::vector<flatbuffers::Offset<my_model::Operator>>{op});
    auto subgraph = my_model::CreateSubGraph(builder, 0, operators);

    auto subgraphs = builder.CreateVector(std::vector<flatbuffers::Offset<my_model::SubGraph>>{subgraph});
    auto model_name = builder.CreateString("SimpleConvModel");
    auto model = my_model::CreateModel(builder, model_name, subgraphs);

    builder.Finish(model);

    std::ofstream outfile(filename, std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), builder.GetSize());
    outfile.close();

    std::cout << "Data saved to " << filename << std::endl;

}


void TestCaseParser::loadBinaryFlatBuffer(const char* filename) {
    std::ifstream infile(filename, std::ios::binary | std::ios::ate);

    if(!infile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if(!infile.read(buffer.data(), size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return;
    }

    auto model = my_model::GetModel(buffer.data());

    std::cout << "Model Name: " << model->model_name()->str() << std::endl;

    for(auto subgraph : *model->subgraphs()) {
        std::cout << "Subgraph Index: " << subgraph->subgraph_index() << std::endl;

        for (auto op : *subgraph->operators()) {
            std::cout << "  Operator Index: " << op->op_index() << std::endl;
            std::cout << "  Operator Name: " << op->op_name()->str() << std::endl;

            auto weights = op->weights();
            std::cout << "    Weights Shape: ";
            for (auto dim : *weights->shape()) {
                std::cout << dim << " ";
            }
            std::cout << std::endl;

            int cnt = 0;
            std::cout << "   Weights Sample 1 ~ 10 : ";
            for(auto kernel_data : *weights->data()) {
                if(cnt >= 10) break;
                std::cout << kernel_data << " ";
                cnt++;
            }
            std::cout << std::endl;

            auto bias = op->bias();
            std::cout << "    Bias Shape: ";
            for (auto dim : *bias->shape()) {
                std::cout << dim << " ";
            }
            std::cout << std::endl;

            std::cout << "    Outputs: ";
            for (auto output : *op->outputs()) {
                std::cout << output << " ";
            }
            std::cout << std::endl;

            for (auto conv_option : *op->conv_options()) {
                std::cout << "    Conv2D Activation Function: " << conv_option->activation_function()->str() << std::endl;
                std::cout << "    Conv2D Padding: " << conv_option->padding()->str() << std::endl;
                std::cout << "    Conv2D Strides: ";
                for (auto stride : *conv_option->strides()) {
                    std::cout << stride << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}
