//
// Created by Duke
//

#include "ImageMatrix.h"
#include "SkMatrix.h"
#include "log.h"

using namespace ImageMatrix;

jlong Matrix::create(JNIEnv *env, jobject clazz, jlong srcHandle) {
    const SkMatrix *src = reinterpret_cast<SkMatrix *>(srcHandle);
    SkMatrix *obj = new SkMatrix();
    if (src)
        *obj = *src;
    else
        obj->reset();
    return reinterpret_cast<jlong>(obj);
}

void Matrix::postRotateF(JNIEnv *env, jobject clazz, jlong objHandle, jfloat degrees) {
    SkMatrix *obj = reinterpret_cast<SkMatrix *>(objHandle);
    obj->postRotate(degrees);
}

void Matrix::postScaleFF(JNIEnv *env, jobject clazz, jlong objHandle, jfloat sx, jfloat sy) {
    SkMatrix *obj = reinterpret_cast<SkMatrix *>(objHandle);
    obj->postScale(sx, sy);
}

void Matrix::destory(JNIEnv *env, jobject clazz, jlong objHandle) {
    SkMatrix *obj = reinterpret_cast<SkMatrix *>(objHandle);
    if (obj)
        delete obj;
}
