#ifndef _GG_
#define _GG_

#include <jni.h>
#include <SkRect.h>
#include <SkPixelRef.h>
#include <SkRegion.h>
#include <SkRasterizer.h>
#include <SkBRDAllocator.h>

enum JNIAccess {
    kRO_JNIAccess,
    kRW_JNIAccess
};

class AutoJavaFloatArray {
public:
    AutoJavaFloatArray(JNIEnv *env, jfloatArray array,
                       int minLength = 0, JNIAccess access = kRW_JNIAccess) :
            fEnv(env), fArray(array), fPtr(NULL), fLen(0) {
        SkASSERT(env);
        if (array) {
            fLen = env->GetArrayLength(array);
            if (fLen < minLength) {
                sk_throw();
            }
            fPtr = env->GetFloatArrayElements(array, NULL);
        }
        fReleaseMode = (access == kRO_JNIAccess) ? JNI_ABORT : 0;
    }

    ~AutoJavaFloatArray() {
        if (fPtr) {
            fEnv->ReleaseFloatArrayElements(fArray, fPtr, fReleaseMode);
        }
    }


    float *ptr() const { return fPtr; }

    int length() const { return fLen; }

private:
    JNIEnv *fEnv;
    jfloatArray fArray;
    float *fPtr;
    int fLen;
    int fReleaseMode;
};

class AutoJavaIntArray {
public:
    AutoJavaIntArray(JNIEnv *env, jintArray array, int minLength = 0)
            : fEnv(env), fArray(array), fPtr(NULL), fLen(0) {
        SkASSERT(env);
        if (array) {
            fLen = env->GetArrayLength(array);
            if (fLen < minLength) {
                sk_throw();
            }
            fPtr = env->GetIntArrayElements(array, NULL);
        }
    }

    ~AutoJavaIntArray() {
        if (fPtr) {
            fEnv->ReleaseIntArrayElements(fArray, fPtr, 0);
        }
    }


    jint *ptr() const { return fPtr; }

    int length() const { return fLen; }

private:
    JNIEnv *fEnv;
    jintArray fArray;
    jint *fPtr;
    int fLen;
};

class AutoJavaShortArray {
public:
    AutoJavaShortArray(JNIEnv *env, jshortArray array,
                       int minLength = 0, JNIAccess access = kRW_JNIAccess)
            : fEnv(env), fArray(array), fPtr(NULL), fLen(0) {
        SkASSERT(env);
        if (array) {
            fLen = env->GetArrayLength(array);
            if (fLen < minLength) {
                sk_throw();
            }
            fPtr = env->GetShortArrayElements(array, NULL);
        }
        fReleaseMode = (access == kRO_JNIAccess) ? JNI_ABORT : 0;
    }

    ~AutoJavaShortArray() {
        if (fPtr) {
            fEnv->ReleaseShortArrayElements(fArray, fPtr, fReleaseMode);
        }
    }


    jshort *ptr() const { return fPtr; }

    int length() const { return fLen; }

private:
    JNIEnv *fEnv;
    jshortArray fArray;
    jshort *fPtr;
    int fLen;
    int fReleaseMode;
};

class AutoJavaByteArray {
public:
    AutoJavaByteArray(JNIEnv *env, jbyteArray array, int minLength = 0)
            : fEnv(env), fArray(array), fPtr(NULL), fLen(0) {
        SkASSERT(env);
        if (array) {
            fLen = env->GetArrayLength(array);
            if (fLen < minLength) {
                sk_throw();
            }
            fPtr = env->GetByteArrayElements(array, NULL);
        }
    }

    ~AutoJavaByteArray() {
        if (fPtr) {
            fEnv->ReleaseByteArrayElements(fArray, fPtr, 0);
        }
    }

    jbyte *ptr() const { return fPtr; }

    int length() const { return fLen; }

private:
    JNIEnv *fEnv;
    jbyteArray fArray;
    jbyte *fPtr;
    int fLen;
};

#endif //_GG_