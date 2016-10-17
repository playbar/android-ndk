cmake_minimum_required(VERSION 3.4.1)

add_library(Echo SHARED
            Echo.cpp)

# Include libraries needed for Echo lib
target_link_libraries(Echo
                      android
                      log)
