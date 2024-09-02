#include "parser.h"

// void FBParser::loadFBFromFile(const char* filename) {
//     ifstream infile(filename, ios::binary | ios::ate );
//     streamsize size = infile.tellg();
//     infile.seekg(0, ios::beg);

//     vector<char> buffer(size);
//     auto model = my_model::GetModel(buffer.data());
// }


void FBParser::printSampleFBFromFile(const char* filename) {

    // ios::ate = file end point
    ifstream infile(filename, ios::binary | ios::ate );

    if(!infile){
        cerr << "failed to open file" << endl;
        exit(EXIT_FAILURE);
    }

    //stream buf position indicator
    streamsize size = infile.tellg();

    //move to file stream  
    //ios::beg  = file start point
    infile.seekg(0, ios::beg);

    vector<char> buffer(size);
    if(!infile.read(buffer.data(), size)){
        cerr << "failed to read flatbuffers" << endl;
        exit(EXIT_FAILURE);
    }

    auto model = my_model::GetModel(buffer.data());
    
    cout << "Model name  : " << model->model_name()->str() << endl;

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