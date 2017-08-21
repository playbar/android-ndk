
// Sample3.c
#include "com_study_jnilearn_Sample3.h"
#include <string.h>
#include <stdlib.h>

/*
 * Class:     com_study_jnilearn_Sample3
 * Method:    accessInstanceField
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample3_accessInstanceField
  (JNIEnv *env, jclass obj)
{
	static jfieldID fid;	// 使用时缓存字段ID
	jstring j_str;
	const char* c_str;
	if (fid == NULL)
	{
		jclass cls = (*env)->GetObjectClass(env,obj);
		fid = (*env)->GetFieldID(env,cls,"str","Ljava/lang/String;");
		if (fid == NULL)
		{
			return;	// not found the field
		}
	}

	j_str = (jstring)(*env)->GetObjectField(env,obj,fid);
	c_str = (*env)->GetStringUTFChars(env,j_str,NULL);	// 从java虚拟机获取一个字符串，会分配内存
	if (c_str == NULL)
	{
		return;	// out of memory
	}
	printf("In C:%s\n",c_str);
	(*env)->ReleaseStringUTFChars(env,j_str,c_str);
	j_str = (*env)->NewStringUTF(env,"hello,i am c");
	if (j_str == NULL)
	{
		return;		// out of memory
	}
	// 修改字段的值
	(*env)->SetObjectField(env,obj,fid,j_str);
}

jmethodID MID_com_study_jnilearn_Sample3_callback;

/*
 * 库加载的时候缓存字段和方法ID
 * Class:     com_study_jnilearn_Sample3
 * Method:    initIds
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample3_initIds
	(JNIEnv *env, jclass cls)
{
	MID_com_study_jnilearn_Sample3_callback = (*env)->GetMethodID(env,cls,"callback","()V");
}

/*
 * Class:     com_study_jnilearn_Sample3
 * Method:    nativeMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample3_nativeMethod
  (JNIEnv *env, jobject obj)
{
	(*env)->CallVoidMethod(env,obj,MID_com_study_jnilearn_Sample3_callback);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    printf("call JNI_OnLoad.....\n");
//    JNIEnv *env;
//    jclass cls;
//    cached_jvm = jvm;  /* cache the JavaVM pointer */
//    if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2)) {
//        return JNI_ERR; /* JNI version not supported */
//    }
//    cls = (*env)->FindClass(env, "C");
//    if (cls == NULL) {
//        return JNI_ERR;
//    }
//    /* Use weak global ref to allow C class to be unloaded */
//    Class_C = (*env)->NewWeakGlobalRef(env, cls);
//    if (Class_C == NULL) {
//        return JNI_ERR;
//    }
//    /* Compute and cache the method ID */
//    MID_C_g = (*env)->GetMethodID(env, cls, "g", "()V");
//    if (MID_C_g == NULL) {
//        return JNI_ERR;
//    }
    return JNI_VERSION_1_2;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *jvm, void *reserved)
{
	printf("JNI_OnUnload call...\n");
	JNIEnv *env;
	if ((*jvm)->GetEnv(jvm, (void **) &env, JNI_VERSION_1_2))
	{
		return;
	}
	//(*env)->DeleteWeakGlobalRef(env, Class_C);
	return;
}
