cmake_minimum_required(VERSION 3.4.1)

add_library(hello-jni SHARED
            hello-jni.c
            load.c
            arrtest.cc)

# Include libraries needed for hello-jni lib
target_link_libraries(hello-jni
                      android
                      log)
