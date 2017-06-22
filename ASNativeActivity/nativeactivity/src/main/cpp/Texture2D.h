#ifndef __INSTANCING_H__
#define __INSTANCING_H__

#include "android/log.h"
#include "esUtil.h"

#define LOG_TAG "native"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

int esMain ( ESContext *esContext );

#ifdef __cplusplus
}
#endif
#endif