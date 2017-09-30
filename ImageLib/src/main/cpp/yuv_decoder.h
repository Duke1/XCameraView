#include <jni.h>
#include "log.h"


#ifndef __YUV_DECODER__
#define __YUV_DECODER__
#ifdef __cplusplus
extern "C" {
#endif

void YUVtoRBGA(signed char* yuv, int width, int height, int* rgbData);

void YUVtoARBG(signed char* yuv, int width, int height, int* rgbData);

#ifdef __cplusplus
}
#endif
#endif
