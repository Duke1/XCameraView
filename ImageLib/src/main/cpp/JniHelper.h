//
// Created by Duke
//

#ifndef _JNIHELPER_H
#define _JNIHELPER_H

#include <jni.h>
#include <string>
#include "log.h"

static inline int
RegisterMethods(JNIEnv *env, const char *className, const JNINativeMethod *gMethods) {

    jclass clazz = NULL;

    clazz = env->FindClass(className);
    if (!clazz) {
        LOGE("JNI_OnLoad->FindClass error!");
        return -1;
    }

    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        LOGE("JNI_OnLoad->RegisterNatives error!");
        return -1;
    }
    return 1;
}

#endif //_JNIHELPER_H
