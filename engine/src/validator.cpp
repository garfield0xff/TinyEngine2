#include "validator.h"

/* 
    i is mean input so i_width = input_width
    k is mena kerenl so k_width = kernel_width
*/
void Validator::validate_cnn1x1_PAD_RELU6(
    float* input, const uint8_t i_width, const uint8_t i_height, const uint8_t i_ch,
    const float* k_weights, size_t k_size, int k_height, int k_width,
    const float* bias,
    float* output, const uint16_t o_width, const uint16_t o_height, const uint16_t o_ch
)
{
    int pad_h = 0, pad_w = 0;
    int pad_x = i_width + 2 * pad_w;
    int pad_y = i_height + 2 * pad_h;

    std::vector<float> col_data(i_ch * o_height * o_width, 0.0f);

    for (int ic = 0; ic < i_ch; ++ic) {
        for (int oh = 0; oh < o_height; ++oh) {
            for (int ow = 0; ow < o_width; ++ow) {
                int input_idx = (oh + pad_h) * pad_x * i_ch + (ow + pad_w) * i_ch + ic;
                int col_idx = ic * o_height * o_width + oh * o_width + ow;
                col_data[col_idx] = input[input_idx];
            }
        }
    }
    
    const int tile_size = 4;

    // Loop Unrolling, Operation Fusion, Tiling 
    for (int oc = 0; oc < o_ch; oc += tile_size) {
        for (int oh = 0; oh < o_height; ++oh) {
            for (int ow = 0; ow < o_width; ++ow) {
                float sums[tile_size] = {0};

                for (int t = 0; t < tile_size && oc + t < o_ch; ++t) {
                    int i_index = oh * o_width + ow;
                    const float* weight_ptr = k_weights + (oc + t) * k_size;

                    // 루프 언롤링 및 연산 수행
                    for (int ic = 0; ic <= i_ch - 4; ic += 4) {
                        sums[t] += col_data[i_index * i_ch + ic] * weight_ptr[ic] +
                                   col_data[i_index * i_ch + ic + 1] * weight_ptr[ic + 1] +
                                   col_data[i_index * i_ch + ic + 2] * weight_ptr[ic + 2] +
                                   col_data[i_index * i_ch + ic + 3] * weight_ptr[ic + 3];
                    }

                    for (int ic = (i_ch / 4) * 4; ic < i_ch; ++ic) {
                        sums[t] += col_data[i_index * i_ch + ic] * weight_ptr[ic];
                    }

                    sums[t] += bias[oc + t];
                    sums[t] = std::max(0.0f, std::min(sums[t], 6.0f));

                    output[(oc + t) * o_height * o_width + oh * o_width + ow] = sums[t];
                }
            }
        }
    }
}


void Validator::validate_depthwise_cnn1x1_PAD_RELU6(
    float* input, const uint8_t i_width, const uint8_t i_height, const uint8_t i_ch,
    const float* k_weights, size_t k_size, int k_height, int k_width,
    const float* bias,
    float* output, const uint16_t o_width, const uint16_t o_height, const uint16_t o_ch
)
{
    int pad_h = 0, pad_w = 0;
    int pad_x = i_width + 2 * pad_w;
    int pad_y = i_height + 2 * pad_h;

    std::vector<float> col_data(i_ch * o_height * o_width, 0.0f);
    // im2col
    for (int ic = 0; ic < i_ch; ++ic) {
        for (int oh = 0; oh < o_height; ++oh) {
            for (int ow = 0; ow < o_width; ++ow) {
                int input_idx = (oh + pad_h) * pad_x * i_ch + (ow + pad_w) * i_ch + ic;
                int col_idx = ic * o_height * o_width + oh * o_width + ow;
                col_data[col_idx] = input[input_idx];
            }
        }
    }

    // 타일 크기 (임의의 값, 하드웨어에 맞춰 조정 가능)
    const int tile_size = 4;

    // Loop Unrolling, Operation Fusion, Tiling 
    for (int ic = 0; ic < i_ch; ic += tile_size) {
        for (int oh = 0; oh < o_height; ++oh) {
            for (int ow = 0; ow < o_width; ++ow) {
                // loop unorlling
                float sums[tile_size] = {0};

                for (int t = 0; t < tile_size && ic + t < i_ch; ++t) {
                    int col_idx = (ic + t) * o_height * o_width + oh * o_width + ow;
                    const float* weight_ptr = k_weights + (ic + t) * k_size;
                    
                    sums[t] = col_data[col_idx] * weight_ptr[0];
                    sums[t] += bias[ic + t];
                    sums[t] = std::max(0.0f, std::min(sums[t], 6.0f));

                    output[(ic + t) * o_height * o_width + oh * o_width + ow] = sums[t];
                }
            }
        }
    }
}