cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

add_library(TestAudio SHARED
            AudioRecordActivity.cpp)

# Include libraries needed for TestAudio lib
target_link_libraries(TestAudio
                      android
                      log
                      OpenSLES)
