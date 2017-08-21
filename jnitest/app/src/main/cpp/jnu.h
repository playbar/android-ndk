//
//  jnu.h
//  JNI
//
//  Created by YangXin on 14/11/2.
//  Copyright (c) 2014年 yangxin. All rights reserved.
//

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef JNI_jnu_h
#define JNI_jnu_h

static jmethodID mid_java_lang_String_getBytes;
static jclass class_java_lang_String;
static jmethodID mid_java_lang_String_init;

static jboolean isInit = JNI_FALSE;

#define CHECK_INIT(...) if (!isInit) { \
    if (__VA_ARGS__) \
    { \
        return NULL; \
    } \
    return; \
}

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * 初始化工具类用到的常用字段ID、方法ID
	 */
	void JNU_Init(JNIEnv *env)
	{
		if (isInit)
		{
			fprintf(stderr,"JNU Already init.\n");
			return;
		}
        isInit = JNI_TRUE;
        class_java_lang_String = (*env)->FindClass(env,"java/lang/String");
        mid_java_lang_String_getBytes = (*env)->GetMethodID(env,class_java_lang_String,"getBytes","()[B");
        mid_java_lang_String_init = (*env)->GetMethodID(env,class_java_lang_String,"<init>","([B)V");
	}
    
    /**
     * 方法调用
	 * 参数：
     * env  : JNIEnv对象
     * hasException : true表示JNI调用有错,否则没有异常
	 * obj  : 调用方法的实例
	 * name : 方法名称
	 * sig  : 方法签名
     */
    jvalue JNU_CallMethodByName(JNIEnv* env, jboolean *hasException,jobject obj,
                                const char* name, const char* sig,...)
    {
        va_list args;
        jvalue result;
        jmethodID mid;
        jclass cls;
        if ((*env)->EnsureLocalCapacity(env,2) != JNI_OK)   // 确保可以创建2个局部引用变量
        {
            return result;
        }
        cls = (*env)->GetObjectClass(env,obj);
        mid = (*env)->GetMethodID(env,cls,name,sig);
        if (mid == NULL)
        {
            return result;
        }
        
        const char*p = sig;
        while (*p != ')') p++;
        p++;
        va_start(args, sig);
        switch (*p) {
            case 'V':   // void
                (*env)->CallVoidMethod(env,obj,mid,args);
                break;
            case 'L':
            case '[':
                result.l = (*env)->CallObjectMethodV(env,obj,mid,args);
                break;
            case 'Z':   // boolean
                result.z = (*env)->CallBooleanMethodV(env,obj,mid,args);
                break;
            case 'I':   // int
                result.i = (*env)->CallIntMethodV(env,obj,mid,args);
                break;
            case 'S':   // short
                result.s = (*env)->CallShortMethodV(env,obj,mid,args);
                break;
            case 'J':   // long
                result.j = (*env)->CallLongMethodV(env,obj,mid,args);
                break;
            case 'D':   // double
                result.d = (*env)->CallDoubleMethodV(env,obj,mid,args);
                break;
            case 'F':   // float
                result.f = (*env)->CallFloatMethodV(env,obj,mid,args);
                break;
            case 'C':   // char
                result.c = (*env)->CallCharMethodV(env,obj,mid,args);
                break;
            case 'B':   // byte
                result.b = (*env)->CallByteMethodV(env,obj,mid,args);
                break;
            default:
                (*env)->FatalError(env,"illeagal method description.");
                break;
        }
        va_end(args);
        (*env)->DeleteLocalRef(env,cls);
        
        if (hasException) {
            *hasException = (*env)->ExceptionCheck(env);
        }
        return result;
    }
    
    /*
	* 抛出一个异常
	* 参数：
	* env : 当前线程的JNI函数表指针
	* className : Java的类全名，用/分割路径
	* msg : 异常描述信息
	*/
    void JNU_ThrowByName(JNIEnv* env, const char* className, const char* msg)
    {
        jthrowable exec;
        exec = (*env)->FindClass(env,className);    // 如果未找到传进来的class，JAVA虚拟机会报一个NowClassDefFoundError和ClassNotFoundException异常
        if (exec != NULL)
        {
            (*env)->ThrowNew(env,exec,msg); // 手动抛出异常
        }
        // 删除局部引用，如果exec为NULL，DeleteLocalRef不受影响
        (*env)->DeleteLocalRef(env,exec);
    }

	/*
	 * 获取一个JNI函数表指针
	*/
	JNIEnv *JNU_GetEnv(JavaVM *jvm)
	{
		if (jvm == NULL)
		{
			fprintf(stderr, "call JNU_GetEnv error, vm is NULL.\n");
			return NULL;
		}
		JNIEnv *env;
		(*jvm)->GetEnv(jvm,(void**)&env,JNI_VERSION_1_4);
		return env;
	}

	/*
	 * 将Java字符串转换成C风格的字符串
	*/
	char *JNU_GetStringNativeChars(JNIEnv *env, jstring jstr)
	{
		jbyteArray byteArray;
		char *result = NULL;
		jthrowable exce;
		jint len;
		if ((*env)->EnsureLocalCapacity(env,2) != JNI_OK)
		{
			return NULL;
		}
		byteArray = (*env)->CallObjectMethod(env,jstr,mid_java_lang_String_getBytes);
		exce = (*env)->ExceptionOccurred(env);
		if (!exce)
		{
			len = (*env)->GetArrayLength(env,byteArray);
			result = (char*)malloc(len + 1);
			if (result == NULL)
			{
				(*env)->DeleteLocalRef(env,byteArray);
				(*env)->DeleteLocalRef(env,exce);
				JNU_ThrowByName(env,"java/lang/OutOfMemoryError","Out of Memory.");
				return NULL;
			}
			(*env)->SetByteArrayRegion(env,byteArray,0,len,(jbyte*)result);
			result[len] = 0;
		}
		else
		{
			(*env)->DeleteLocalRef(env,exce);
			return NULL;
		}
		(*env)->DeleteLocalRef(env,exce);
		(*env)->DeleteLocalRef(env,byteArray);
		return result;
	}
    
    // 将C字符串转换成JAVA字符串
    jstring JNU_NewStringNative(JNIEnv *env, const char *str)
    {
        jstring result = NULL;
        jbyteArray bytes = NULL;
        jint len = 0;
        if ((*env)->EnsureLocalCapacity(env,2) != JNI_OK)
        {
            return NULL;
        }
        len = strlen(str);
        bytes = (*env)->NewByteArray(env,len);
        if (bytes != NULL)
        {
            (*env)->SetByteArrayRegion(env,bytes,0,len,(const jbyte*)str);
            result = (*env)->NewObject(env,class_java_lang_String,mid_java_lang_String_init,bytes);
            (*env)->DeleteLocalRef(env,bytes);
            return result;
        }
        return NULL;
    }
    
#ifdef __cplusplus
}
#endif

#endif
