
#include <jni.h>
#include <vector>
#include "../log.h"

#include "Graphics.h"
#include "NinePatchPeeker.h"
#include <SkAndroidCodec.h>
#include "SkImageEncoder.h"
#include "SkCanvas.h"
#include "SkSurface.h"
#include "gl/GrGLInterface.h"
#include "GrContext.h"


#include <android/bitmap.h>

#include "Image.h"


jboolean Image::Image::bitmap_compress(JNIEnv *env, jobject clazz, jobject bitmapin,
                                       jint format, jint quality, jbyteArray jstorage,
                                       jstring savePath) {
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


jboolean
Image::Image::bitmap_save(JNIEnv *env, jobject clazz, jint format, jbyteArray byteArray,
                          jint offset,
                          jint length, jint quality, jstring savePath) {

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

    AutoJavaByteArray ar(env, byteArray);
    std::unique_ptr<SkMemoryStream> stream(new SkMemoryStream(ar.ptr() + offset, length, false));

    std::unique_ptr<SkStreamRewindable> streamDeleter(stream.release());

    int sampleSize = 1;
//    SkColorType prefColorType = kN32_SkColorType;
//    float scale = 1.0f;
//    sk_sp<SkColorSpace> prefColorSpace = nullptr;

    // Create the codec.
    NinePatchPeeker peeker;
    std::unique_ptr<SkAndroidCodec> codec(
            SkAndroidCodec::NewFromStream(streamDeleter.release(), &peeker));
    if (!codec.get()) {
        LOGE("SkAndroidCodec::NewFromStream returned null");
        return JNI_FALSE;
    }

    // Determine the output size.
    SkISize size = codec->getSampledDimensions(sampleSize);

    // Set the decode colorType
//    SkColorType decodeColorType = codec->computeOutputColorType(prefColorType);
//    sk_sp<SkColorSpace> decodeColorSpace = codec->computeOutputColorSpace(decodeColorType,prefColorSpace);

    SkAndroidCodec::AndroidOptions opts;
    opts.fSampleSize = 1;
    auto info = codec->getInfo().makeWH(size.fWidth, size.fHeight).makeColorType(kN32_SkColorType);
    SkBitmap *bm = new SkBitmap();
    bm->allocPixels(info);
    codec->getAndroidPixels(info, bm->getPixels(), bm->rowBytes(), &opts);



    //encode and save to file
    char *fileName = (char *) env->GetStringUTFChars(savePath, JNI_FALSE);
    SkWStream *outputStream = new SkFILEWStream(fileName);
    env->ReleaseStringUTFChars(savePath, fileName);
    int isSuccess;

    if (1) {
        const SkRect g_rtImg = SkRect::MakeXYWH(0, 0, bm->width(), bm->height());
        SkRect deviceR;

        SkMatrix matrix;
        matrix.reset();
        matrix.postRotate(90);
        matrix.mapRect(&deviceR, g_rtImg);

        SkImageInfo imageInfo = SkImageInfo::MakeS32(deviceR.width(), deviceR.height(),
                                                     kPremul_SkAlphaType);
        imageInfo.makeColorType(kN32_SkColorType);

//        SkBitmap *newBitmap = new SkBitmap();
//        newBitmap->allocPixels(imageInfo);//

        SkPaint *paint = new SkPaint();
        paint->setFilterQuality(kNone_SkFilterQuality);
        paint->setAntiAlias(true);

        //1
//        SkCanvas *canvas = new SkCanvas(*newBitmap);
        //2 cpu
        sk_sp<SkSurface> surface = SkSurface::MakeRaster(imageInfo);
        //3 gpu
//        sk_sp<const GrGLInterface> glInterface(GrGLCreateNativeInterface());
//        GrContext *context = GrContext::Create(kOpenGL_GrBackend,(GrBackendContext) glInterface.get());
//        sk_sp<SkSurface> surface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, imageInfo));

        if (!surface) {
            LOGE("SkSurface::MakeRenderTarget returned null\n");
            return JNI_FALSE;
        }

        SkCanvas *canvas = surface->getCanvas();
        canvas->save();
        canvas->translate(-deviceR.left(), -deviceR.top());
        canvas->concat(matrix);
        canvas->drawBitmapRect(*bm, g_rtImg, g_rtImg, paint);
        canvas->restore();


        sk_sp<SkImage> img(surface->makeImageSnapshot());
        sk_sp<SkData> data(img->encode(fm, quality));

        isSuccess = outputStream->write(data->data(), data->size()) ? JNI_TRUE : JNI_FALSE;
        delete paint;
    } else {
        isSuccess =
                SkEncodeImage(outputStream/*strm.get()*/, *bm, fm, quality) ? JNI_TRUE : JNI_FALSE;
    }

    if (bm)
        delete bm;

    outputStream->flush();
    delete outputStream;

    return isSuccess;
}


