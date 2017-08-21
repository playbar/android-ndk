// Sample1.c

#include "com_study_jnilearn_Sample1.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    intMethod
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_study_jnilearn_Sample1_intMethod
  (JNIEnv *env, jobject obj, jint num)
{
	return num * num;
}

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    booleanMethod
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_study_jnilearn_Sample1_booleanMethod
  (JNIEnv *env, jobject obj, jboolean b)
{
	return !b;
}

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    floatMethod
 * Signature: (F)F
 */
JNIEXPORT jfloat JNICALL Java_com_study_jnilearn_Sample1_floatMethod
  (JNIEnv *env, jobject obj, jfloat f)
{
	return f + f;
}

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    stringMethod
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_study_jnilearn_Sample1_stringMethod1
  (JNIEnv *env, jobject obj, jstring j_str)
{
	const char* c_str;
	char buff[128];
	memset(buff,0,sizeof(buff));
	jboolean isCopy;	// 返回JNI_TRUE表示原字符串的拷贝，返回JNI_FALSE表示返回原字符串的指针
	c_str = (*env)->GetStringUTFChars(env,j_str,&isCopy);	// 将java虚拟机中的字符串转换C字符串，并分配内存空间
	printf("isCopy:%d\n",isCopy);
	if (c_str == NULL) {
		return NULL;
	}
	(*env)->ReleaseStringUTFChars(env,j_str,c_str);	// 释放从java虚拟机中获取的字符串内存空间
	sprintf(buff,"hello %s",(char*)c_str);
	// 创建一个C语言中的字符串，并转换成unicode编码
	return (*env)->NewStringUTF(env,buff);
}

// GetStringCritical访问Java字符串指针
JNIEXPORT jstring JNICALL Java_com_study_jnilearn_Sample1_stringMethod2
  (JNIEnv *env, jobject obj, jstring j_str)
{
	// 用GetStringCritical实现
	const jchar* c_str= NULL;
	char buff[128] = "hello ";
	char* pBuff = buff + 6;
	/*
	 * 在GetStringCritical/RealeaseStringCritical之间是一个关键区。
	 * 在这关键区之中,绝对不能呼叫JNI的其他函数和会造成当前线程中断或是会让当前线程等待的任何本地代码，
	 * 否则将造成关键区代码执行区间垃圾回收器停止运作，任何触发垃圾回收器的线程也会暂停。
	 * 其他触发垃圾回收器的线程不能前进直到当前线程结束而激活垃圾回收器。
	 */
	c_str = (*env)->GetStringCritical(env,j_str,NULL);	// 返回字符串指针的可能性
	if (c_str == NULL)
	{
		return NULL;
	}
	while(*c_str) {	// 将java字符串通过指针拷贝到缓冲区中
		*pBuff++ = *c_str++;
	}
	//printf("%s\n",buff);
	//sprintf(buff,"hello %s",c_str);
	(*env)->ReleaseStringCritical(env,j_str,c_str);
	return (*env)->NewStringUTF(env,buff);
}

// GetStringUTFRegion
JNIEXPORT jstring JNICALL Java_com_study_jnilearn_Sample1_stringMethod
  (JNIEnv *env, jobject obj, jstring j_str)
{
	jsize len = (*env)->GetStringLength(env,j_str);	// 获取unicode字符串的长度（java是以用unicode编码的）
	printf("\nstr len:%d\n",len);
	char buff[128] = "hello ";
	char* pBuff = buff + 6;
	// 将java中的字符串以utf-8编码拷入C缓冲区,该函数内部不会分配内存空间
	(*env)->GetStringUTFRegion(env,j_str,0,len,pBuff);
	//(*env)->GetStringRegion(env,j_str,0,len,pBuff);	// 以utf16编码拷与c缓冲区
	return (*env)->NewStringUTF(env,buff);
}

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    intArrayMethod
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_com_study_jnilearn_Sample1_intArrayMethod1
  (JNIEnv *env, jobject obj, jintArray array)
{
	jint *carr;
	jint i,sum = 0;
	jsize len = (*env)->GetArrayLength(env,array);
	carr = (*env)->GetIntArrayElements(env,array,NULL);
	if(carr == NULL)
	{
		return 0;
	}

	for(i = 0; i < len; i++)
	{
		sum += carr[i];
	}
	/**
	 * 第三个参数
	 * 0：更新JAVA数组，并释放本地内存
	 * 1：更新JAVA数组，但不释放本地内存
	 * 2：不更新JAVA数组，释放本地内存
	 */
	(*env)->ReleaseIntArrayElements(env,array,carr,0);	// 释放内存
	return sum;
}

// 计算奇数的和
JNIEXPORT jint JNICALL Java_com_study_jnilearn_Sample1_intArrayMethod
  (JNIEnv *env, jobject obj, jintArray j_arr)
{
	int i, sum = 0;
	// 获取数组的长度
	jsize arr_len = (*env)->GetArrayLength(env,j_arr);
	jsize count = arr_len / 2;
	jint *c_arr = (jint*)malloc(sizeof(jint) * count);
	// 拷贝java数组指定区域的元素到本地缓冲区中
	(*env)->GetIntArrayRegion(env,j_arr,0,count,c_arr);
	// 计算奇数的和
	for(i = 0; i < count; i++)
	{
		if (c_arr[i] % 2 != 0)
		{
			sum += c_arr[i];
		}
	}
	free(c_arr);
	c_arr = NULL;
	return sum;
}

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    stringArrayMethod
 * Signature: ([Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_study_jnilearn_Sample1_stringArrayMethod
  (JNIEnv *env, jobject obj, jobjectArray j_arr, jstring j_str_separator)
{
	// 获取数组元素分割符
	const char* c_str_separator;
	jsize len;
	c_str_separator = (*env)->GetStringUTFChars(env,j_str_separator,NULL);
	if(c_str_separator == NULL)
	{
		return NULL;
	}

	len = (*env)->GetArrayLength(env,j_arr);	// 获取数组的长度
	printf("str len:%d",len);
	// 将数组元素用分隔连接成字符串
	int i;
	char buff[1024];
	memset(buff,0,sizeof(buff));
	for(i = 0; i < len; i++)
	{
		jstring j_str = (*env)->GetObjectArrayElement(env,j_arr,i);
		const char* c_str = (*env)->GetStringUTFChars(env,j_str,NULL);
		if(c_str == NULL)
		{
			continue;
		}
		(*env)->ReleaseStringUTFChars(env,j_str,c_str);
		strcat(buff,c_str);
		if (i < len - 1)
			strcat(buff,c_str_separator);
	}
	return (*env)->NewStringUTF(env,buff);
}

/*
 * Class:     com_study_jnilearn_Sample1
 * Method:    init2array
 * Signature: (I)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_com_study_jnilearn_Sample1_init2array
  (JNIEnv *env, jobject obj, jint size)
{
	jobjectArray result;
	int i;
	jclass intArrCls = (*env)->FindClass(env,"[I");
	if (intArrCls == NULL)
	{
		return NULL;
	}

	result = (*env)->NewObjectArray(env,size,intArrCls,NULL);	// 实例化一个int一维数组
	if(result == NULL)
	{
		return NULL;
	}

	for (i = 0; i < size; i++)
	{
		jint tmp[256];
		int j;
		jintArray iarr = (*env)->NewIntArray(env,size);	// 创建一个int数组
		if (iarr == NULL)
		{
			return NULL;	// out of memory error
		}

		for (j = 0; j < size; j++)
		{
			tmp[j] = i + j;	// 将数据拷贝到临时数组中
		}
		(*env)->SetIntArrayRegion(env, iarr, 0, size, tmp);	// 将临时数组中的数据拷贝到新创建的数组中
		(*env)->SetObjectArrayElement(env,result,i,iarr);	// 设置一维数组的第i个元素的值为iarr
		(*env)->DeleteLocalRef(env,iarr);	// 释放局部变量
	}
	return result;
}

#ifdef __cplusplus
}
#endif
