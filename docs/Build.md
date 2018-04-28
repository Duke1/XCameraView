
##Skia
请参考:https://skia.org/user/build
1. 先执行：python tools/git-sync-deps 获取gn
2. 配置：
   bin/gn gen out/arm      --args='ndk="/home/xiaoyaozi/Android/Sdk/ndk-bundle" target_cpu="arm" target_os="android" is_component_build=false is_debug=false ndk_api=16 is_official_build=true extra_cflags_cc=["-Wno-unused-variable"] skia_enable_pdf=false skia_enable_tools=false skia_use_system_libpng=false skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_expat=false skia_use_system_freetype2=false skia_use_fontconfig=false skia_enable_jumper=true'
   查看配置：bin/gn args --list out/arm
   
3. 编译
   ninja -C out/arm
   
   
   
##OpenCV
1. 把[下载](https://sourceforge.net/projects/opencvlibrary/files/opencv-android/)的zip里的sdk文件夹解压到OpenCV目录下
2. 修改CMakeLists.txt
    `#OpenCV配置
     SET(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../OpenCV/sdk/native/jni)
     find_package(OpenCV REQUIRED)
     message("OpenCV Version: ${OpenCV_VERSION}")
    `
    
    `target_link_libraries(${OpenCV_LIBS})   
     include_directories(${OpenCV_INCLUDE_DIRS}                          )
    `