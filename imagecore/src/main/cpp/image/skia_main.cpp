
#include <jni.h>
#include <vector>
#include "../log.h"

#include "Graphics.h"
#include "NinePatchPeeker.h"
#include "CreateJavaOutputStreamAdaptor.h"

#include <SkStream.h>
#include <SkAndroidCodec.h>


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


#include <stdio.h>
#include <image/skia/include/core/SkImageEncoder.h>
#include <ImageData.h>


static jobject doDecode(JNIEnv *env, SkStreamRewindable *stream, jobject padding, jobject options) {
    // This function takes ownership of the input stream.  Since the SkAndroidCodec
    // will take ownership of the stream, we don't necessarily need to take ownership
    // here.  This is a precaution - if we were to return before creating the codec,
    // we need to make sure that we delete the stream.
    std::unique_ptr<SkStreamRewindable> streamDeleter(stream);

    // Set default values for the options parameters.
    int sampleSize = 1;
    bool onlyDecodeSize = false;
    SkColorType prefColorType = kN32_SkColorType;
    bool isHardware = false;
    bool isMutable = false;
    float scale = 1.0f;
    bool requireUnpremultiplied = false;
    jobject javaBitmap = NULL;
    sk_sp<SkColorSpace> prefColorSpace = nullptr;

//    // Update with options supplied by the client.
//    if (options != NULL) {
//        sampleSize = env->GetIntField(options, gOptions_sampleSizeFieldID);
//        // Correct a non-positive sampleSize.  sampleSize defaults to zero within the
//        // options object, which is strange.
//        if (sampleSize <= 0) {
//            sampleSize = 1;
//        }
//
//        if (env->GetBooleanField(options, gOptions_justBoundsFieldID)) {
//            onlyDecodeSize = true;
//        }
//
//        // initialize these, in case we fail later on
//        env->SetIntField(options, gOptions_widthFieldID, -1);
//        env->SetIntField(options, gOptions_heightFieldID, -1);
//        env->SetObjectField(options, gOptions_mimeFieldID, 0);
//        env->SetObjectField(options, gOptions_outConfigFieldID, 0);
//        env->SetObjectField(options, gOptions_outColorSpaceFieldID, 0);
//
//        jobject jconfig = env->GetObjectField(options, gOptions_configFieldID);
//        prefColorType = GraphicsJNI::getNativeBitmapColorType(env, jconfig);
//        jobject jcolorSpace = env->GetObjectField(options, gOptions_colorSpaceFieldID);
//        prefColorSpace = GraphicsJNI::getNativeColorSpace(env, jcolorSpace);
//        isHardware = GraphicsJNI::isHardwareConfig(env, jconfig);
//        isMutable = env->GetBooleanField(options, gOptions_mutableFieldID);
//        requireUnpremultiplied = !env->GetBooleanField(options, gOptions_premultipliedFieldID);
//        javaBitmap = env->GetObjectField(options, gOptions_bitmapFieldID);
//
//        if (env->GetBooleanField(options, gOptions_scaledFieldID)) {
//            const int density = env->GetIntField(options, gOptions_densityFieldID);
//            const int targetDensity = env->GetIntField(options, gOptions_targetDensityFieldID);
//            const int screenDensity = env->GetIntField(options, gOptions_screenDensityFieldID);
//            if (density != 0 && targetDensity != 0 && density != screenDensity) {
//                scale = (float) targetDensity / density;
//            }
//        }
//    }
//
//    if (isMutable && isHardware) {
//        doThrowIAE(env, "Bitmaps with Config.HARWARE are always immutable");
//        return nullObjectReturn("Cannot create mutable hardware bitmap");
//    }

    // Create the codec.
    NinePatchPeeker peeker;
    std::unique_ptr<SkAndroidCodec> codec(SkAndroidCodec::NewFromStream(
            streamDeleter.release(), &peeker));
    if (!codec.get()) {
//        return nullObjectReturn("SkAndroidCodec::NewFromStream returned null");
    }

    // Do not allow ninepatch decodes to 565.  In the past, decodes to 565
    // would dither, and we do not want to pre-dither ninepatches, since we
    // know that they will be stretched.  We no longer dither 565 decodes,
    // but we continue to prevent ninepatches from decoding to 565, in order
    // to maintain the old behavior.
//    if (peeker.mPatch && kRGB_565_SkColorType == prefColorType) {
//        prefColorType = kN32_SkColorType;
//    }

    // Determine the output size.
    SkISize size = codec->getSampledDimensions(sampleSize);

    int scaledWidth = size.width();
    int scaledHeight = size.height();
    bool willScale = false;
//
//    // Apply a fine scaling step if necessary.
//    if (needsFineScale(codec->getInfo().dimensions(), size, sampleSize)) {
//        willScale = true;
//        scaledWidth = codec->getInfo().width() / sampleSize;
//        scaledHeight = codec->getInfo().height() / sampleSize;
//    }

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

//
//    // Set the options and return if the client only wants the size.
//    if (options != NULL) {
//        jstring mimeType = encodedFormatToString(
//                env, (SkEncodedImageFormat)codec->getEncodedFormat());
//        if (env->ExceptionCheck()) {
//            return nullObjectReturn("OOM in encodedFormatToString()");
//        }
//        env->SetIntField(options, gOptions_widthFieldID, scaledWidth);
//        env->SetIntField(options, gOptions_heightFieldID, scaledHeight);
//        env->SetObjectField(options, gOptions_mimeFieldID, mimeType);
//
//        SkColorType outColorType = decodeColorType;
//        // Scaling can affect the output color type
//        if (willScale || scale != 1.0f) {
//            outColorType = colorTypeForScaledOutput(outColorType);
//        }
//
//        jint configID = GraphicsJNI::colorTypeToLegacyBitmapConfig(outColorType);
//        if (isHardware) {
//            configID = GraphicsJNI::kHardware_LegacyBitmapConfig;
//        }
//        jobject config = env->CallStaticObjectMethod(gBitmapConfig_class,
//                                                     gBitmapConfig_nativeToConfigMethodID, configID);
//        env->SetObjectField(options, gOptions_outConfigFieldID, config);
//
//        env->SetObjectField(options, gOptions_outColorSpaceFieldID,
//                            GraphicsJNI::getColorSpace(env, decodeColorSpace, decodeColorType));
//
//        if (onlyDecodeSize) {
//            return nullptr;
//        }
//    }
//
//    // Scale is necessary due to density differences.
//    if (scale != 1.0f) {
//        willScale = true;
//        scaledWidth = static_cast<int>(scaledWidth * scale + 0.5f);
//        scaledHeight = static_cast<int>(scaledHeight * scale + 0.5f);
//    }
//
//    android::Bitmap* reuseBitmap = nullptr;
//    unsigned int existingBufferSize = 0;
//    if (javaBitmap != NULL) {
//        reuseBitmap = &bitmap::toBitmap(env, javaBitmap);
//        if (reuseBitmap->isImmutable()) {
//            ALOGW("Unable to reuse an immutable bitmap as an image decoder target.");
//            javaBitmap = NULL;
//            reuseBitmap = nullptr;
//        } else {
//            existingBufferSize = bitmap::getBitmapAllocationByteCount(env, javaBitmap);
//        }
//    }
//
//    HeapAllocator defaultAllocator;
//    RecyclingPixelAllocator recyclingAllocator(reuseBitmap, existingBufferSize);
//    ScaleCheckingAllocator scaleCheckingAllocator(scale, existingBufferSize);
//    SkBitmap::HeapAllocator heapAllocator;
//    SkBitmap::Allocator* decodeAllocator;
//    if (javaBitmap != nullptr && willScale) {
//        // This will allocate pixels using a HeapAllocator, since there will be an extra
//        // scaling step that copies these pixels into Java memory.  This allocator
//        // also checks that the recycled javaBitmap is large enough.
//        decodeAllocator = &scaleCheckingAllocator;
//    } else if (javaBitmap != nullptr) {
//        decodeAllocator = &recyclingAllocator;
//    } else if (willScale || isHardware) {
//        // This will allocate pixels using a HeapAllocator,
//        // for scale case: there will be an extra scaling step.
//        // for hardware case: there will be extra swizzling & upload to gralloc step.
//        decodeAllocator = &heapAllocator;
//    } else {
//        decodeAllocator = &defaultAllocator;
//    }
//
//    // Construct a color table for the decode if necessary
//    sk_sp<SkColorTable> colorTable(nullptr);
//    SkPMColor* colorPtr = nullptr;
//    int* colorCount = nullptr;
//    int maxColors = 256;
//    SkPMColor colors[256];
//    if (kIndex_8_SkColorType == decodeColorType) {
//        colorTable.reset(new SkColorTable(colors, maxColors));
//
//        // SkColorTable expects us to initialize all of the colors before creating an
//        // SkColorTable.  However, we are using SkBitmap with an Allocator to allocate
//        // memory for the decode, so we need to create the SkColorTable before decoding.
//        // It is safe for SkAndroidCodec to modify the colors because this SkBitmap is
//        // not being used elsewhere.
//        colorPtr = const_cast<SkPMColor*>(colorTable->readColors());
//        colorCount = &maxColors;
//    }
//
//    SkAlphaType alphaType = codec->computeOutputAlphaType(requireUnpremultiplied);
//
//    const SkImageInfo decodeInfo = SkImageInfo::Make(size.width(), size.height(),
//                                                     decodeColorType, alphaType, decodeColorSpace);
//
//    // For wide gamut images, we will leave the color space on the SkBitmap.  Otherwise,
//    // use the default.
//    SkImageInfo bitmapInfo = decodeInfo;
//    if (decodeInfo.colorSpace() && decodeInfo.colorSpace()->isSRGB()) {
//        bitmapInfo = bitmapInfo.makeColorSpace(GraphicsJNI::colorSpaceForType(decodeColorType));
//    }
//
//    if (decodeColorType == kGray_8_SkColorType) {
//        // The legacy implementation of BitmapFactory used kAlpha8 for
//        // grayscale images (before kGray8 existed).  While the codec
//        // recognizes kGray8, we need to decode into a kAlpha8 bitmap
//        // in order to avoid a behavior change.
//        bitmapInfo =
//                bitmapInfo.makeColorType(kAlpha_8_SkColorType).makeAlphaType(kPremul_SkAlphaType);
//    }
//    SkBitmap decodingBitmap;
//    if (!decodingBitmap.setInfo(bitmapInfo) ||
//        !decodingBitmap.tryAllocPixels(decodeAllocator, colorTable.get())) {
//        // SkAndroidCodec should recommend a valid SkImageInfo, so setInfo()
//        // should only only fail if the calculated value for rowBytes is too
//        // large.
//        // tryAllocPixels() can fail due to OOM on the Java heap, OOM on the
//        // native heap, or the recycled javaBitmap being too small to reuse.
//        return nullptr;
//    }
//
//    // Use SkAndroidCodec to perform the decode.
//    SkAndroidCodec::AndroidOptions codecOptions;
//    codecOptions.fZeroInitialized = decodeAllocator == &defaultAllocator ?
//                                    SkCodec::kYes_ZeroInitialized : SkCodec::kNo_ZeroInitialized;
//    codecOptions.fColorPtr = colorPtr;
//    codecOptions.fColorCount = colorCount;
//    codecOptions.fSampleSize = sampleSize;
//    SkCodec::Result result = codec->getAndroidPixels(decodeInfo, decodingBitmap.getPixels(),
//                                                     decodingBitmap.rowBytes(), &codecOptions);
//    switch (result) {
//        case SkCodec::kSuccess:
//        case SkCodec::kIncompleteInput:
//            break;
//        default:
//            return nullObjectReturn("codec->getAndroidPixels() failed.");
//    }
//
//    jbyteArray ninePatchChunk = NULL;
//    if (peeker.mPatch != NULL) {
//        if (willScale) {
//            scaleNinePatchChunk(peeker.mPatch, scale, scaledWidth, scaledHeight);
//        }
//
//        size_t ninePatchArraySize = peeker.mPatch->serializedSize();
//        ninePatchChunk = env->NewByteArray(ninePatchArraySize);
//        if (ninePatchChunk == NULL) {
//            return nullObjectReturn("ninePatchChunk == null");
//        }
//
//        jbyte* array = (jbyte*) env->GetPrimitiveArrayCritical(ninePatchChunk, NULL);
//        if (array == NULL) {
//            return nullObjectReturn("primitive array == null");
//        }
//
//        memcpy(array, peeker.mPatch, peeker.mPatchSize);
//        env->ReleasePrimitiveArrayCritical(ninePatchChunk, array, 0);
//    }
//
//    jobject ninePatchInsets = NULL;
//    if (peeker.mHasInsets) {
//        ninePatchInsets = env->NewObject(gInsetStruct_class, gInsetStruct_constructorMethodID,
//                                         peeker.mOpticalInsets[0], peeker.mOpticalInsets[1],
//                                         peeker.mOpticalInsets[2], peeker.mOpticalInsets[3],
//                                         peeker.mOutlineInsets[0], peeker.mOutlineInsets[1],
//                                         peeker.mOutlineInsets[2], peeker.mOutlineInsets[3],
//                                         peeker.mOutlineRadius, peeker.mOutlineAlpha, scale);
//        if (ninePatchInsets == NULL) {
//            return nullObjectReturn("nine patch insets == null");
//        }
//        if (javaBitmap != NULL) {
//            env->SetObjectField(javaBitmap, gBitmap_ninePatchInsetsFieldID, ninePatchInsets);
//        }
//    }
//
//    SkBitmap outputBitmap;
//    if (willScale) {
//        // This is weird so let me explain: we could use the scale parameter
//        // directly, but for historical reasons this is how the corresponding
//        // Dalvik code has always behaved. We simply recreate the behavior here.
//        // The result is slightly different from simply using scale because of
//        // the 0.5f rounding bias applied when computing the target image size
//        const float sx = scaledWidth / float(decodingBitmap.width());
//        const float sy = scaledHeight / float(decodingBitmap.height());
//
//        // Set the allocator for the outputBitmap.
//        SkBitmap::Allocator* outputAllocator;
//        if (javaBitmap != nullptr) {
//            outputAllocator = &recyclingAllocator;
//        } else {
//            outputAllocator = &defaultAllocator;
//        }
//
//        SkColorType scaledColorType = colorTypeForScaledOutput(decodingBitmap.colorType());
//        // FIXME: If the alphaType is kUnpremul and the image has alpha, the
//        // colors may not be correct, since Skia does not yet support drawing
//        // to/from unpremultiplied bitmaps.
//        outputBitmap.setInfo(
//                bitmapInfo.makeWH(scaledWidth, scaledHeight).makeColorType(scaledColorType));
//        if (!outputBitmap.tryAllocPixels(outputAllocator, NULL)) {
//            // This should only fail on OOM.  The recyclingAllocator should have
//            // enough memory since we check this before decoding using the
//            // scaleCheckingAllocator.
//            return nullObjectReturn("allocation failed for scaled bitmap");
//        }
//
//        SkPaint paint;
//        // kSrc_Mode instructs us to overwrite the uninitialized pixels in
//        // outputBitmap.  Otherwise we would blend by default, which is not
//        // what we want.
//        paint.setBlendMode(SkBlendMode::kSrc);
//        paint.setFilterQuality(kLow_SkFilterQuality); // bilinear filtering
//
//        SkCanvas canvas(outputBitmap, SkCanvas::ColorBehavior::kLegacy);
//        canvas.scale(sx, sy);
//        canvas.drawBitmap(decodingBitmap, 0.0f, 0.0f, &paint);
//    } else {
//        outputBitmap.swap(decodingBitmap);
//    }
//
//    if (padding) {
//        if (peeker.mPatch != NULL) {
//            GraphicsJNI::set_jrect(env, padding,
//                                   peeker.mPatch->paddingLeft, peeker.mPatch->paddingTop,
//                                   peeker.mPatch->paddingRight, peeker.mPatch->paddingBottom);
//        } else {
//            GraphicsJNI::set_jrect(env, padding, -1, -1, -1, -1);
//        }
//    }
//
//    // If we get here, the outputBitmap should have an installed pixelref.
//    if (outputBitmap.pixelRef() == NULL) {
//        return nullObjectReturn("Got null SkPixelRef");
//    }
//
//    if (!isMutable && javaBitmap == NULL) {
//        // promise we will never change our pixels (great for sharing and pictures)
//        outputBitmap.setImmutable();
//    }
//
//    bool isPremultiplied = !requireUnpremultiplied;
//    if (javaBitmap != nullptr) {
//        bitmap::reinitBitmap(env, javaBitmap, outputBitmap.info(), isPremultiplied);
//        outputBitmap.notifyPixelsChanged();
//        // If a java bitmap was passed in for reuse, pass it back
//        return javaBitmap;
//    }
//
//    int bitmapCreateFlags = 0x0;
//    if (isMutable) bitmapCreateFlags |= android::bitmap::kBitmapCreateFlag_Mutable;
//    if (isPremultiplied) bitmapCreateFlags |= android::bitmap::kBitmapCreateFlag_Premultiplied;
//
//    if (isHardware) {
//        sk_sp<Bitmap> hardwareBitmap = Bitmap::allocateHardwareBitmap(outputBitmap);
//        return bitmap::createBitmap(env, hardwareBitmap.release(), bitmapCreateFlags,
//                                    ninePatchChunk, ninePatchInsets, -1);
//    }
//
//    // now create the java bitmap
//    return bitmap::createBitmap(env, defaultAllocator.getStorageObjAndReset(),
//                                bitmapCreateFlags, ninePatchChunk, ninePatchInsets, -1);

    return NULL;
}

static jboolean Bitmap_compress(JNIEnv *env, jobject clazz, jobject bitmapin,
                                jint format, jint quality,
                                jobject jstream, jbyteArray jstorage) {
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


//    std::unique_ptr<SkWStream> strm(/*CreateJavaOutputStreamAdaptor(env, jstream, jstorage)*/);
//    if (!strm.get()) {
//        return JNI_FALSE;
//    }


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
    SkImageInfo imageInfo = SkImageInfo::MakeS32(1, 1, kOpaque_SkAlphaType);
    imageInfo.makeWH(bitmapInfoIn.width, bitmapInfoIn.height);
    imageInfo.makeColorType(kN32_SkColorType);
    skbitmap.allocPixels(imageInfo);
    skbitmap.setPixels(pixelsIn);

    SkWStream *stream = new SkFILEWStream("/sdcard/ACamera/aa.jpeg");
    result = SkEncodeImage(stream/*strm.get()*/, skbitmap, fm, quality) ? JNI_TRUE : JNI_FALSE;
    stream->flush();
    delete stream;
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
        {       "nativeCompress",        "(Landroid/graphics/Bitmap;IILjava/io/OutputStream;[B)Z",
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

