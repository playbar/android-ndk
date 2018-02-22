//
// Created by hou guoli on 2018/2/22.
//

#include "javahello.h"
#include <stdio.h>

/**
 * 定义native方法
 */


JNIEXPORT jstring JNICALL native_hello(JNIEnv *env, jclass clazz)
{
    printf("hello in c native code./n");
    return env->NewStringUTF("hello world returned.");
}

// 指定要注册的类
#define JNIREG_CLASS "com/example/JavaHello"

// 定义一个JNINativeMethod数组，其中的成员就是Java代码中对应的native方法
static JNINativeMethod gMethods[] = {
        { "hello", "()Ljava/lang/String;", (void*)native_hello},
};


static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives( clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/***
 * 注册native方法
 */
int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env, JNIREG_CLASS, gMethods, sizeof(gMethods) / sizeof(gMethods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
