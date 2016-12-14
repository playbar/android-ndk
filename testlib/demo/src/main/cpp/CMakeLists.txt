cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -fno-exceptions -fno-rtti")

# build the ndk-helper library
set(ndk_helper_dir ../../../../common/ndk_helper)
add_subdirectory(${ndk_helper_dir} ndk_helper)

# now build app's shared lib
add_library(svrapitest SHARED
            app.cpp)

target_include_directories(svrapitest PRIVATE
                           ${ANDROID_NDK}/sources/android/cpufeatures
                           ${ANDROID_NDK}/sources/android/native_app_glue
                           ${ndk_helper_dir})

# add lib dependencies
target_link_libraries(svrapitest
                      android
                      app-glue
                      atomic
                      EGL
                      GLESv2
                      log
                      ndk-helper)
