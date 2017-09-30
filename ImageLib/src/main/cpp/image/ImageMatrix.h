//
// Created by Duke
//

#ifndef _IMAGECORE_IMAGEMATRIX_H
#define _IMAGECORE_IMAGEMATRIX_H

#include <jni.h>

namespace ImageMatrix {
    class Matrix {
    public:
        static jlong create(JNIEnv *env, jobject clazz, jlong srcHandle);

        static void postRotateF(JNIEnv *env, jobject clazz, jlong objHandle, jfloat degrees);

        static void postScaleFF(JNIEnv *env, jobject clazz, jlong objHandle, jfloat sx, jfloat sy);

        static void destory(JNIEnv *env, jobject clazz, jlong objHandle);

    };

    static const JNINativeMethod methods[] = {
            {"nCreate",     "(J)J",   (void *) Matrix::create},
            {"nDestory",    "(J)V",   (void *) Matrix::destory},
            {"nPostRotate", "(JF)V",  (void *) Matrix::postRotateF},
            {"nPostScale",  "(JFF)V", (void *) Matrix::postScaleFF}
    };

    const static char *name = "xyz/openhh/imagecore/ImageMatrix";
}
#endif //_IMAGECORE_IMAGEMATRIX_H
