#include "parser.h"
#include "validator.h"

#include <iostream>


using namespace std;

int main() {
    // FBParser fb_p1;
    // const char* binary_path = "/Users/gyujinkim/Desktop/Ai/TinyEngine2/Model2FB/backbone.bin";

    // fb_p1.printSampleFBFromFile(binary_path);

    Validator validator;

    const uint8_t i_width = 224, i_height = 224, i_ch = 3;
    const uint16_t o_width = 112, o_height = 112, o_ch = 32;

    std::vector<float> input(i_width * i_height * i_ch, 2.0f);  
    std::vector<float> k_weights(o_ch * i_ch, 0.112122f);  
    std::vector<float> bias(o_ch, 0.123123f);  
    std::vector<float> output(o_width * o_height * o_ch, 0.123123f);  

    auto start_time = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 40; i++) {
            validator.validate_cnn1x1_PAD_RELU6(
            input.data(), i_width, i_height, i_ch,
            k_weights.data(), k_weights.size(), 1, 1,
            bias.data(),
            output.data(), o_width, o_height, o_ch
            );
        }
        for(int i = 0; i < 17; i++)
        {
            validator.validate_depthwise_cnn1x1_PAD_RELU6(
                input.data(), i_width, i_height, i_ch,
                k_weights.data(), k_weights.size(), 1, 1,
                bias.data(),
                output.data(), o_width, o_height, o_ch
            );
        }
    


    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "CNN 1x1 PAD + ReLU6 연산 시간: " << elapsed.count() << " 초" << std::endl;

    return 0;
}
