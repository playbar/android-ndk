// JNIException.c

#include "com_study_jnilearn_JNIException.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_com_study_jnilearn_JNIException_doit
  (JNIEnv *env, jclass cls) {
	jthrowable exc = NULL;
	jmethodID mid = (*env)->GetStaticMethodID(env,cls,"exceptionCallback","()V");
	if (mid != NULL) {
		(*env)->CallStaticVoidMethod(env,cls,mid);
	}
//	printf("In C: Java_com_study_jnilearn_JNIException_doit-->called!!!!");
//	if ((*env)->ExceptionCheck(env)) {	// 检查JNI调用是否已引发异常
//		(*env)->ExceptionDescribe(env);
//		(*env)->ExceptionClear(env);	// 清除引发的异常，在Java层不会打印异常的堆栈信息
//		(*env)->ThrowNew(env,(*env)->FindClass(env,"java/lang/Exception"),"JNI抛出的异常！");
//		return;
//	}
      
	exc = (*env)->ExceptionOccurred(env);
	if (exc) {
		(*env)->ExceptionDescribe(env);	// 打印Java层抛出的异常堆栈信息
		(*env)->ExceptionClear(env);	// 清除异常信息

		// 在本地代码中手动抛出一个异常
		jclass newExcCls;
		newExcCls = (*env)->FindClass(env,"java/lang/IllegalArgumentException");
		if (newExcCls == NULL) {
			return;
		}
		(*env)->ThrowNew(env, newExcCls, "throw from C Code.");
		return;
	}

	mid = (*env)->GetStaticMethodID(env,cls,"normalCallback","()V");
	if (mid != NULL) {
		(*env)->CallStaticVoidMethod(env,cls,mid);
	}
}
