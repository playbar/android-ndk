cmake_minimum_required(VERSION 3.4.1)

add_library(WAVPlayer SHARED
            WAVPlayer.cpp
            avilib/avilib.c
            avilib/wavlib.c
            avilib/platform_posix.c)

# Include libraries needed for Threads lib
target_link_libraries(WAVPlayer
                      android
                      OpenSLES
                      log)
