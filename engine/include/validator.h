#ifndef _Validator_
#define _Validator_

#include "parser.h"

class Validator {
    public:
        void validate_cnn1x1_PAD_RELU6(
            float *input, const uint8_t x, const uint8_t y, const uint8_t ch,
            const float *k_weights, size_t k_size, int k_height, int k_width,
            const float *bias,
            float *output, const uint16_t o_width, const uint16_t o_height, const uint16_t o_ch
        );

        void validate_depthwise_cnn1x1_PAD_RELU6(
            float* input, const uint8_t i_width, const uint8_t i_height, const uint8_t i_ch,
            const float* k_weights, size_t k_size, int k_height, int k_width,
            const float* bias,
            float* output, const uint16_t o_width, const uint16_t o_height, const uint16_t o_ch
        );
};

#endif

