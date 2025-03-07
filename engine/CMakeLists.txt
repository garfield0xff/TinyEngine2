cmake_minimum_required(VERSION 3.10)
project(TensorFlowLiteExample)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TFLITE_DIR "tensorflow-lite/tensorflow-master")
set(FLATBUFFERS_INCLUDE_DIR "/usr/local/include")
set(FLATBUFFERS_LIB_DIR "/usr/local/lib")

include_directories(${TFLITE_DIR})
include_directories(${TFLITE_DIR}/tensorflow/lite)
include_directories(${TFLITE_DIR}/tensorflow/lite/tools/make/downloads/flatbuffers/include)

add_library(tensorflowlite STATIC
    ${TFLITE_DIR}/tensorflow/lite/core/api/error_reporter.cc
    ${TFLITE_DIR}/tensorflow/lite/core/api/op_resolver.cc
    ${TFLITE_DIR}/tensorflow/lite/interpreter.cc
    ${TFLITE_DIR}/tensorflow/lite/kernels/kernel_util.cc
    ${TFLITE_DIR}/tensorflow/lite/model.cc
    ${TFLITE_DIR}/tensorflow/lite/kernels/register.cc
    ${TFLITE_DIR}/tensorflow/lite/tools/make/downloads/flatbuffers/src/flatbuffers.cpp
)

add_executable(TensorFlowLiteExample src/main.cpp)

target_link_libraries(TensorFlowLiteExample tensorflowlite flatbuffers  ${OpenCV_LIBS})
target_include_directories(TensorFlowLiteExample PRIVATE ${PROJECT_SOURCE_DIR}/include ${FLATBUFFERS_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
