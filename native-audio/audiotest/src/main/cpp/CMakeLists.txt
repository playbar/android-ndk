cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

add_library(opensl_example SHARED
           opensl_example.c
           opensl_io.c
           java_interface_wrap.cpp )

# Include libraries needed for native-audio-jni lib
target_link_libraries(opensl_example
                      android
                      log
                      OpenSLES)
