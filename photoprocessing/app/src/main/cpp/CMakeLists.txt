cmake_minimum_required(VERSION 3.4.1)

add_library(photoprocessing SHARED
                nanojpeg.c
                mem_utils.c
                bitmap.c
                bicubic_resize.c
                filter.c
                transform.c
                colour_space.c
                matrix.c
                blur.c
                photo_processing.c
            )

# Include libraries needed for hello-jni lib
target_link_libraries(photoprocessing
                      m
                      log)
