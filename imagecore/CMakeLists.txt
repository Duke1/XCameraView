# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


SET(SKIA_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)

message(lib目录：${SKIA_LIB_PATH})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             imageengine

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             #src/main/cpp/image/CreateJavaOutputStreamAdaptor.cpp
             src/main/cpp/image/NinePatchPeeker.cpp
             src/main/cpp/image/skia_main.cpp
             src/main/cpp/yuv_decoder.cpp
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       imageengine

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}

                       android

                       jnigraphics

                       ${SKIA_LIB_PATH}/image/skia/${ANDROID_ABI}/libskia.a

                       )


include_directories(
                 ${SKIA_LIB_PATH}
                 ${SKIA_LIB_PATH}/image
                 ${SKIA_LIB_PATH}/image/skia/include
                 ${SKIA_LIB_PATH}/image/skia/include/android
                 ${SKIA_LIB_PATH}/image/skia/include/c
                 ${SKIA_LIB_PATH}/image/skia/include/codec
                 ${SKIA_LIB_PATH}/image/skia/include/config
                 ${SKIA_LIB_PATH}/image/skia/include/core
                 ${SKIA_LIB_PATH}/image/skia/include/effects
                 ${SKIA_LIB_PATH}/image/skia/include/gpu
                 ${SKIA_LIB_PATH}/image/skia/include/gpu/gl
                 ${SKIA_LIB_PATH}/image/skia/include/pathops
                 ${SKIA_LIB_PATH}/image/skia/include/ports
                 ${SKIA_LIB_PATH}/image/skia/include/svg
                 ${SKIA_LIB_PATH}/image/skia/include/utils
                 ${SKIA_LIB_PATH}/image/skia/include/utils/mac
                  )