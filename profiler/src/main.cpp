#include <iostream>
#include <stddef.h>
#include <opencv2/opencv.hpp>

#include "profiler.h"

using namespace cv;

int main(void)
{
    const char* img_path = "/Users/gyujinkim/Desktop/Ai/TinyEngine/code_generator/test/notPerson1.png";
    
    Mat img = imread(img_path);

    Mat resize_img;
    resize(img, resize_img, Size(224, 224));
    resize_img.convertTo(resize_img, CV_32F, 1.0 / 255.0);
    Mat blob = resize_img.reshape(1, 1);
    blob = dnn::blobFromImage(blob);

    uint32_t imageBuffer_size = resize_img.cols * resize_img.rows * resize_img.channels();
    
    float* imageBuffer = new float[imageBuffer_size];


    int batch_size = blob.size[0];

    for(int i = 0; i < imageBuffer_size; ++i) {
        imageBuffer[i] = resize_img.at<float>(i);  
    }

    profiler p;

    while(1){
        p.displayMenu(&p);
    }

    delete[] imageBuffer;

    return 0;
}