//
// Created by Duke
//

#ifndef _IMAGECORE_IMAGE_H
#define _IMAGECORE_IMAGE_H


#include <jni.h>

namespace Image {

    typedef struct bitmap_info {
        int width;
        int height;
        int bit_deep_num;//byte
    } BitmapInfo;

    enum JavaEncodeFormat {
        kJPEG_JavaEncodeFormat = 0,
        kPNG_JavaEncodeFormat = 1,
        kWEBP_JavaEncodeFormat = 2
    };


    class Image {
    public:
        static jboolean bitmap_compress(JNIEnv *env, jobject clazz, jobject bitmapin,
                                        jint format, jint quality, jbyteArray jstorage,
                                        jstring savePath);

        static jboolean
        bitmap_save(JNIEnv *env, jobject clazz, jint format, jbyteArray byteArray, jint offset,
                    jint length, jint quality, jstring savePath);
    };

    static const JNINativeMethod methods[] = {
            {
                    "nativeSave",
                    "(I[BIIILjava/lang/String;)Z",
                    (void *) Image::bitmap_save
            },
            {
                    "nativeCompress",
                    "(Landroid/graphics/Bitmap;II[BLjava/lang/String;)Z",
                    (void *) Image::bitmap_compress
            }
    };

    const static char *name = "xyz/openhh/imagecore/Image";
}

#endif //_IMAGECORE_IMAGE_H
