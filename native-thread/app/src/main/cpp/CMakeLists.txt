cmake_minimum_required(VERSION 3.4.1)

add_library(Threads SHARED
            nativethread.cpp)

# Include libraries needed for Threads lib
target_link_libraries(Threads
                      android
                      log)
