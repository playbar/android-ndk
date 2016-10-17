cmake_minimum_required(VERSION 3.4.1)

add_library(AVIPlayer SHARED
            AbstractPlayerActivity.cpp
            BitmapPlayerActivity.cpp
            NativeWindowPlayerActivity.cpp
            Common.cpp
            avilib/avilib.c
            avilib/platform_posix.c)

# Include libraries needed for Threads lib
target_link_libraries(AVIPlayer
                      android
                      jnigraphics
                      log)
