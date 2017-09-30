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

        static void postRotateF(jlong objHandle, jfloat degrees);

        static void postScaleFF(jlong objHandle, jfloat sx, jfloat sy);
    };

    static const JNINativeMethod methods[] = {
            {"nCreate",     "(J)J",   (void *) Matrix::create},
            {"nPostRotate", "(JF)V",  (void *) Matrix::postRotateF},
            {"nPostScale",  "(JFF)V", (void *) Matrix::postScaleFF}
    };

    const static char *name = "xyz/openhh/imagecore/ImageMatrix";
}
#endif //_IMAGECORE_IMAGEMATRIX_H
