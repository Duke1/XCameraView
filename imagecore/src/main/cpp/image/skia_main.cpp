
#include <jni.h>
#include <vector>
#include "../log.h"

#include "Graphics.h"
#include "NinePatchPeeker.h"
#include <SkAndroidCodec.h>
#include "SkImageEncoder.h"


#include <android/bitmap.h>

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


static jobject doDecode(JNIEnv *env, SkStreamRewindable *stream, jobject padding, jobject options) {
    std::unique_ptr<SkStreamRewindable> streamDeleter(stream);

    int sampleSize = 1;
    bool onlyDecodeSize = false;
    SkColorType prefColorType = kN32_SkColorType;
    bool isHardware = false;
    bool isMutable = false;
    float scale = 1.0f;
    bool requireUnpremultiplied = false;
    sk_sp<SkColorSpace> prefColorSpace = nullptr;

    // Create the codec.
    NinePatchPeeker peeker;
    std::unique_ptr<SkAndroidCodec> codec(SkAndroidCodec::NewFromStream(
            streamDeleter.release(), &peeker));
    if (!codec.get()) {
//        return nullObjectReturn("SkAndroidCodec::NewFromStream returned null");
    }

    // Determine the output size.
    SkISize size = codec->getSampledDimensions(sampleSize);

    // Set the decode colorType
    SkColorType decodeColorType = codec->computeOutputColorType(prefColorType);
    sk_sp<SkColorSpace> decodeColorSpace = codec->computeOutputColorSpace(
            decodeColorType, prefColorSpace);

    SkAndroidCodec::AndroidOptions opts;
    opts.fSampleSize = 1;
    auto info = codec->getInfo().makeWH(size.fWidth, size.fHeight).makeColorType(kN32_SkColorType);
    SkBitmap bm;
    bm.allocPixels(info);
    auto result = codec->getAndroidPixels(info, bm.getPixels(), bm.rowBytes(), &opts);

    SkWStream *outputStream = new SkFILEWStream("/sdcard/ACamera/a11.jpeg");
    SkEncodeImage(outputStream/*strm.get()*/, bm, SkEncodedImageFormat::kJPEG, 100) ? JNI_TRUE
                                                                                    : JNI_FALSE;
    outputStream->flush();
    delete outputStream;

    return NULL;
}


static jboolean Bitmap_compress(JNIEnv *env, jobject clazz, jobject bitmapin,
                                jint format, jint quality, jbyteArray jstorage, jstring savePath) {
    SkEncodedImageFormat fm;
    switch (format) {
        case kJPEG_JavaEncodeFormat:
            fm = SkEncodedImageFormat::kJPEG;
            break;
        case kPNG_JavaEncodeFormat:
            fm = SkEncodedImageFormat::kPNG;
            break;
        case kWEBP_JavaEncodeFormat:
            fm = SkEncodedImageFormat::kWEBP;
            break;
        default:
            return JNI_FALSE;
    }

    AndroidBitmapInfo bitmapInfoIn;
    void *pixelsIn;
    int result;

    if ((result = AndroidBitmap_getInfo(env, bitmapin, &bitmapInfoIn)) < 0) {
        LOGE("Get bitmapin information failed ! Error code is:%d", result);
        return JNI_FALSE;
    }

    if (bitmapInfoIn.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmapin format is not RGBA_8888 !");
        return JNI_FALSE;
    }

    if ((result = AndroidBitmap_lockPixels(env, bitmapin, &pixelsIn)) < 0) {
        LOGE("Lock bitmapin failed ! error=%d", result);
    }

    AndroidBitmap_unlockPixels(env, bitmapin);


    SkBitmap skbitmap;
    SkImageInfo imageInfo = SkImageInfo::MakeS32(bitmapInfoIn.width, bitmapInfoIn.height,
                                                 kPremul_SkAlphaType);
    imageInfo.makeColorType(kN32_SkColorType);

    skbitmap.installPixels(imageInfo, pixelsIn, bitmapInfoIn.stride);
    skbitmap.notifyPixelsChanged();


    char *fileName = (char *) env->GetStringUTFChars(savePath, JNI_FALSE);
    SkWStream *stream = new SkFILEWStream(fileName);
    result = SkEncodeImage(stream/*strm.get()*/, skbitmap, fm, quality) ? JNI_TRUE : JNI_FALSE;
    stream->flush();
    delete stream;

    env->ReleaseStringUTFChars(savePath, fileName);
    return result;
}


static jobject nativeDecodeByteArray(JNIEnv *env, jobject, jbyteArray byteArray,
                                     jint offset, jint length, jobject options) {

    AutoJavaByteArray ar(env, byteArray);
    std::unique_ptr<SkMemoryStream> stream(new SkMemoryStream(ar.ptr() + offset, length, false));
    return doDecode(env, stream.release(), NULL, options);
}

static void native_handle(jbyteArray byteArray) {

}

const char *classPathName = "xyz/openhh/imagecore/Image";

static JNINativeMethod methods[] = {
        {
                "compressToJpeg",        "([B)V",                                                                   (void *) native_handle
        },
        {       "nativeDecodeByteArray", "([BIILandroid/graphics/BitmapFactory$Options;)Landroid/graphics/Bitmap;", (void *) nativeDecodeByteArray
        },
        {       "nativeCompress",        "(Landroid/graphics/Bitmap;II[BLjava/lang/String;)Z",
                                                                                                                    (void *) Bitmap_compress}
};


jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    jclass clazz = NULL;
    JNIEnv *env = NULL;


    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI_OnLoad->GetEnv error!");
        return -1;
    }

    clazz = env->FindClass(classPathName);
    if (!clazz) {
        LOGE("JNI_OnLoad->FindClass error!");
        return -1;
    }

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOGE("JNI_OnLoad->RegisterNatives error!");
        return -1;
    }

    return JNI_VERSION_1_6;
}

