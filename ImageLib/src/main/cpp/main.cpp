//
// Created by Duke
//

#include <jni.h>
#include "log.h"

#include "image/Image.h"
#include "image/ImageMatrix.h"

#define RNN(env, className, gMethods)   clazz = env->FindClass(className);\
                                        if (!clazz) {\
                                            LOGE("JNI_OnLoad->FindClass error!");\
                                            return -1;\
                                        }\
                                        if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {\
                                            LOGE("JNI_OnLoad->RegisterNatives error!");\
                                            return -1;\
                                        }\


jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGE("JNI_OnLoad=============Image");

    JNIEnv *env = NULL;


    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI_OnLoad->GetEnv error!");
        return -1;
    }

    //    jclass clazz = NULL;
    //
    //    clazz = env->FindClass(name);
    //    if (!clazz) {
    //        LOGE("JNI_OnLoad->FindClass error!");
    //        return -1;
    //    }
    //
    //    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
    //        LOGE("JNI_OnLoad->RegisterNatives error!");
    //        return -1;
    //    }
    //    registerImageMatrix(env);

    jclass clazz = NULL;

    RNN(env, Image::name, Image::methods);
    RNN(env, ImageMatrix::name, ImageMatrix::methods);

    return JNI_VERSION_1_6;
}
