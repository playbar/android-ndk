/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_apress_echo_EchoServerActivity */

#ifndef _Included_com_apress_echo_EchoServerActivity
#define _Included_com_apress_echo_EchoServerActivity
#ifdef __cplusplus
extern "C" {
#endif
#undef com_apress_echo_EchoServerActivity_MODE_PRIVATE
#define com_apress_echo_EchoServerActivity_MODE_PRIVATE 0L
#undef com_apress_echo_EchoServerActivity_MODE_WORLD_READABLE
#define com_apress_echo_EchoServerActivity_MODE_WORLD_READABLE 1L
#undef com_apress_echo_EchoServerActivity_MODE_WORLD_WRITEABLE
#define com_apress_echo_EchoServerActivity_MODE_WORLD_WRITEABLE 2L
#undef com_apress_echo_EchoServerActivity_MODE_APPEND
#define com_apress_echo_EchoServerActivity_MODE_APPEND 32768L
#undef com_apress_echo_EchoServerActivity_MODE_MULTI_PROCESS
#define com_apress_echo_EchoServerActivity_MODE_MULTI_PROCESS 4L
#undef com_apress_echo_EchoServerActivity_BIND_AUTO_CREATE
#define com_apress_echo_EchoServerActivity_BIND_AUTO_CREATE 1L
#undef com_apress_echo_EchoServerActivity_BIND_DEBUG_UNBIND
#define com_apress_echo_EchoServerActivity_BIND_DEBUG_UNBIND 2L
#undef com_apress_echo_EchoServerActivity_BIND_NOT_FOREGROUND
#define com_apress_echo_EchoServerActivity_BIND_NOT_FOREGROUND 4L
#undef com_apress_echo_EchoServerActivity_BIND_ABOVE_CLIENT
#define com_apress_echo_EchoServerActivity_BIND_ABOVE_CLIENT 8L
#undef com_apress_echo_EchoServerActivity_BIND_ALLOW_OOM_MANAGEMENT
#define com_apress_echo_EchoServerActivity_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef com_apress_echo_EchoServerActivity_BIND_WAIVE_PRIORITY
#define com_apress_echo_EchoServerActivity_BIND_WAIVE_PRIORITY 32L
#undef com_apress_echo_EchoServerActivity_BIND_IMPORTANT
#define com_apress_echo_EchoServerActivity_BIND_IMPORTANT 64L
#undef com_apress_echo_EchoServerActivity_BIND_ADJUST_WITH_ACTIVITY
#define com_apress_echo_EchoServerActivity_BIND_ADJUST_WITH_ACTIVITY 64L
#undef com_apress_echo_EchoServerActivity_CONTEXT_INCLUDE_CODE
#define com_apress_echo_EchoServerActivity_CONTEXT_INCLUDE_CODE 1L
#undef com_apress_echo_EchoServerActivity_CONTEXT_IGNORE_SECURITY
#define com_apress_echo_EchoServerActivity_CONTEXT_IGNORE_SECURITY 2L
#undef com_apress_echo_EchoServerActivity_CONTEXT_RESTRICTED
#define com_apress_echo_EchoServerActivity_CONTEXT_RESTRICTED 4L
#undef com_apress_echo_EchoServerActivity_RESULT_CANCELED
#define com_apress_echo_EchoServerActivity_RESULT_CANCELED 0L
#undef com_apress_echo_EchoServerActivity_RESULT_OK
#define com_apress_echo_EchoServerActivity_RESULT_OK -1L
#undef com_apress_echo_EchoServerActivity_RESULT_FIRST_USER
#define com_apress_echo_EchoServerActivity_RESULT_FIRST_USER 1L
#undef com_apress_echo_EchoServerActivity_DEFAULT_KEYS_DISABLE
#define com_apress_echo_EchoServerActivity_DEFAULT_KEYS_DISABLE 0L
#undef com_apress_echo_EchoServerActivity_DEFAULT_KEYS_DIALER
#define com_apress_echo_EchoServerActivity_DEFAULT_KEYS_DIALER 1L
#undef com_apress_echo_EchoServerActivity_DEFAULT_KEYS_SHORTCUT
#define com_apress_echo_EchoServerActivity_DEFAULT_KEYS_SHORTCUT 2L
#undef com_apress_echo_EchoServerActivity_DEFAULT_KEYS_SEARCH_LOCAL
#define com_apress_echo_EchoServerActivity_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef com_apress_echo_EchoServerActivity_DEFAULT_KEYS_SEARCH_GLOBAL
#define com_apress_echo_EchoServerActivity_DEFAULT_KEYS_SEARCH_GLOBAL 4L
/*
 * Class:     com_apress_echo_EchoServerActivity
 * Method:    nativeStartTcpServer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_apress_echo_EchoServerActivity_nativeStartTcpServer
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_apress_echo_EchoServerActivity
 * Method:    nativeStartUdpServer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_apress_echo_EchoServerActivity_nativeStartUdpServer
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_apress_echo_EchoServerActivity
 * Method:    nativeStartLocalServer
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_apress_echo_EchoServerActivity_nativeStartLocalServer
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
