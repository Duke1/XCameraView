
cmake_minimum_required(VERSION 3.4.1)


SET(NATIVE_CPP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)
message(lib目录：${NATIVE_CPP_PATH})

#OpenCV配置
SET(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../OpenCV/sdk/native/jni)
find_package(OpenCV REQUIRED)
message("OpenCV Version: ${OpenCV_VERSION}")



add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp
             src/main/cpp/CvUtils.cpp
             src/main/cpp/Mat.cpp
              )


find_library( log-lib
              log )


target_link_libraries( native-lib
                       ${log-lib}
                      android

                      jnigraphics

                      dl

                      z
                       ${OpenCV_LIBS})

include_directories(
                ${NATIVE_CPP_PATH}
                ${NATIVE_CPP_PATH}/include
                ${OpenCV_INCLUDE_DIRS}
                 )