cmake_minimum_required(VERSION 3.10)
project(Profiler)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(nlohmann_json 3.11.0 REQUIRED)


set(TFLITE_LIB_PATH "/Users/gyujinkim/Desktop/Ai/TinyEngine/code_generator/tensorflow/bazel-bin/tensorflow/lite")
set(TFLITE_INCLUDE_PATH "/Users/gyujinkim/Desktop/Ai/TinyEngine/code_generator/tensorflow")

set(FLATBUFFERS_INCLUDE_DIR "/opt/homebrew/include")
set(FLATBUFFERS_LIB_DIR "/opt/homebrew/lib")


include_directories(${TFLITE_INCLUDE_PATH} ${FLATBUFFERS_INCLUDE_DIR})
link_directories(${TFLITE_LIB_PATH} ${FLATBUFFERS_LIB_DIR})

include_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCES
    src/cpu.cpp
    src/file.cpp
    src/main.cpp
    src/memory.cpp
    src/profiler.cpp
    src/timer.cpp
)

add_executable(Profiler ${SOURCES})

target_link_libraries(Profiler tensorflowlite flatbuffers  nlohmann_json::nlohmann_json ${OpenCV_LIBS})

target_include_directories(Profiler PRIVATE ${PROJECT_SOURCE_DIR}/include ${FLATBUFFERS_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
