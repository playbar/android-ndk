cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

add_library(native_audio SHARED
            native_audio.c
            opensl_io.c )

# Include libraries needed for native-audio-jni lib
target_link_libraries(native_audio
                      android
                      log
                      OpenSLES)
