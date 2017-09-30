#include <jni.h>
#include "android/log.h"


#ifndef  __UNREAL_CRY_LOG__
#define __UNREAL_CRY_LOG__




#define  LOG_TAG    "Unreal"
//__android_log_print(int prio, const char *tag,  const char *fmt, ...)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#endif
