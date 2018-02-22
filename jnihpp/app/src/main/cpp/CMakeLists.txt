cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -std=c++14 -fsigned-char -Wno-extern-c-compat -fexceptions")


add_library(hello-jni SHARED
            hello.cpp
           )


target_include_directories(hello-jni PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                    )

# Include libraries needed for hello-jni lib
target_link_libraries(hello-jni
                      android
                      log)
