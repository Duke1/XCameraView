#!/usr/bin/env bash


python3 build_sdk.py ./OpencvSdk \
    --ndk_path=${NDK_HOME} \
    --sdk_path=${ANDROID_HOME} \
    --config='ndk-16.config.py' \
    --extra_modules_path=./OpenCV/opencv_contrib-3.4.1/modules \
    --no_ccache
    #--force_opencv_toolchain


    #--sdk_path=${ANDROID_HOME} \

exit 0
