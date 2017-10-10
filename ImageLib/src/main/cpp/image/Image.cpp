
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
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <android/native_window_jni.h>
#include "SkGraphics.h"


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

struct engine {
    struct ANativeWindow *nativeWindow;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;

    GrContext *skiaContext;
    SkCanvas *skiaCanvas;
};

/**
 * Initialize an EGL context for the current display.
 */
static void egl_init_display(struct engine *engine) {
    // Initialize OpenGL ES and EGL

    const EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT/*EGL_WINDOW_BIT*/,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    const EGLint surfaceAttribs[] = {
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, NULL, NULL);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);


    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

//    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
//     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
//     * As soon as we picked a EGLConfig, we can safely reconfigure the
//     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
//    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
//    ANativeWindow_setBuffersGeometry(engine->nativeWindow, 0, 0, format);
//    surface = eglCreateWindowSurface(display, config, engine->nativeWindow, surfaceAttribs);

    static const EGLint kSurfaceAttribs[] = {
            EGL_WIDTH, 1,
            EGL_HEIGHT, 1,
            EGL_NONE
    };
    surface = eglCreatePbufferSurface(display, config, kSurfaceAttribs);


    if (!eglMakeCurrent(display, surface, surface, context)) {
        LOGE("Unable to eglMakeCurrent");
//        return;
    }

//    eglQuerySurface(display, surface, EGL_WIDTH, &w);
//    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
//
//    LOGI("Canvas size: %d x %d", w, h);
//    glViewport(0, 0, w, h);
//
//    engine->display = display;
//    engine->context = context;
//    engine->surface = surface;
//    engine->width = w;
//    engine->height = h;
//
//    // Initialize GL state.
//    glDisable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
////    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
////    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
////    glShadeModel(GL_SMOOTH);
//
//    // eglSwapBuffers should not automatically clear the screen
//    eglSurfaceAttrib(display, surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
//
//    // Initialize Skia OpenGL ES

    SkGraphics::Init();

    const GrGLInterface *fInterface = GrGLCreateNativeInterface();
    engine->skiaContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext) fInterface);

//    GrBackendRenderTargetDesc desc;
//    desc.fWidth       = w;
//    desc.fHeight      = h;
//    desc.fConfig      = kSkia8888_GrPixelConfig;
//    desc.fOrigin      = kBottomLeft_GrSurfaceOrigin;
//    desc.fSampleCnt   = 0;
//    desc.fStencilBits = 8;
//
//    GrGLint buffer;
//    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
//    // Alternative:
//    // GR_GL_GetIntegerv(fInterface, GR_GL_FRAMEBUFFER_BINDING, &buffer);
//    desc.fRenderTargetHandle = buffer;
//
//    GrRenderTarget* renderTarget = engine->skiaContext->wrapBackendRenderTarget(desc);
//    SkAutoTUnref<SkBaseDevice> device(new SkGpuDevice(engine->skiaContext, renderTarget));
//
//    // Leaking fRenderTarget. Either wrap it in an SkAutoTUnref<> or unref it
//    // after creating the device.
//    SkSafeUnref(renderTarget);
//
//    engine->skiaCanvas = new SkCanvas(device);

}

jboolean
Image::Image::bitmap_save(JNIEnv *env, jobject clazz, jint format, jbyteArray byteArray,
                          jint offset,
                          jint length, jint quality, jstring savePath, jlong matrixPtr,
                          jobject javaSurface) {

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

    if (matrixPtr) {
        const SkRect g_rtImg = SkRect::MakeXYWH(0, 0, bm->width(), bm->height());
        SkRect deviceR;

        SkMatrix *matrix = reinterpret_cast<SkMatrix *>(matrixPtr);
        //matrix->reset();
        matrix->mapRect(&deviceR, g_rtImg);

        SkImageInfo imageInfo = SkImageInfo::MakeS32(deviceR.width(), deviceR.height(),
                                                     kPremul_SkAlphaType);
        imageInfo.makeColorType(kN32_SkColorType);

//        SkBitmap *newBitmap = new SkBitmap();
//        newBitmap->allocPixels(imageInfo);//

        SkPaint *paint = new SkPaint();
        paint->setFilterQuality(kNone_SkFilterQuality);
        paint->setAntiAlias(true);

        struct engine engine;
        memset(&engine, 0, sizeof(engine));
        engine.nativeWindow = ANativeWindow_fromSurface(env, javaSurface);
        egl_init_display(&engine);
        GrContext *context = engine.skiaContext;//GrContext::Create(kOpenGL_GrBackend,(GrBackendContext) glInterface.get());
        sk_sp<SkSurface> surface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, imageInfo));

        if (!surface) {
            LOGE("SkSurface::MakeRenderTarget returned null\n");
            surface = SkSurface::MakeRaster(imageInfo);
        }

        SkCanvas *canvas = surface->getCanvas();
        canvas->save();
        canvas->translate(-deviceR.left(), -deviceR.top());
        canvas->concat(*matrix);
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


