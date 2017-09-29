//
// Created by Duke
//

#include "ImageMatrix.h"
#include <JniHelper.h>
#include "SkMatrix.h"


jlong ImageMatrix::Matrix::create(JNIEnv *env, jobject clazz, jlong srcHandle) {
    const SkMatrix *src = reinterpret_cast<SkMatrix *>(srcHandle);
    SkMatrix *obj = new SkMatrix();
    if (src)
        *obj = *src;
    else
        obj->reset();
    return reinterpret_cast<jlong>(obj);
}

void ImageMatrix::Matrix::postRotateF(jlong objHandle, jfloat degrees) {
    SkMatrix *obj = reinterpret_cast<SkMatrix *>(objHandle);
    obj->postRotate(degrees);
}

