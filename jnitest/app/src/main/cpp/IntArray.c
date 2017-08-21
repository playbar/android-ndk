// IntArray.c
#include "com_study_jnilearn_IntArray.h"
#include <string.h>
#include <stdlib.h>

/*
 * Class:     com_study_jnilearn_IntArray
 * Method:    sumArray
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_com_study_jnilearn_IntArray_sumArray1
(JNIEnv *env, jobject obj, jintArray j_array)
{
    jint i, sum = 0;
    jint *c_array;
    jint arr_len;
    // 获取数组长度
    arr_len = (*env)->GetArrayLength(env,j_array);
    // 根据数组长度和数组元素的数据类型申请存放java数组元素的缓冲区
    c_array = (jint*)malloc(sizeof(jint) * arr_len);
    // 初始化缓冲区
    memset(c_array,0,sizeof(jint)*arr_len);
    printf("arr_len = %d \n", arr_len);
    // 拷贝Java数组中的所有元素到缓冲区中
    (*env)->GetIntArrayRegion(env,j_array,0,arr_len,c_array);
    for (i = 0; i < arr_len; i++) {
        sum += c_array[i];  // 累加数组元素的和
    }
    free(c_array);  // 释放存储数组元素的缓冲区
    return sum;
}

JNIEXPORT jint JNICALL Java_com_study_jnilearn_IntArray_sumArray2
(JNIEnv *env, jobject obj, jintArray j_array)
{
    jint i, sum = 0;
    jint *c_array;
    jint arr_len;
    // 可能数组中的元素在内存中是不连续的，JVM可能会复制所有原始数据到缓冲区，然后返回这个缓冲区的指针
    jboolean isCopy;
    c_array = (*env)->GetIntArrayElements(env,j_array,&isCopy);
    printf("isCopy: %d \n", isCopy);
    if (c_array == NULL) {
        return 0;   // JVM复制原始数据到缓冲区失败
    }
    arr_len = (*env)->GetArrayLength(env,j_array);
    printf("arr_len = %d\n", arr_len);
    for (i = 0; i < arr_len; i++) {
        sum += c_array[i];
    }
    (*env)->ReleaseIntArrayElements(env,j_array, c_array, 0);
    return sum;
}

JNIEXPORT jint JNICALL Java_com_study_jnilearn_IntArray_sumArray
(JNIEnv *env, jobject obj, jintArray j_array)
{
    jint i, sum = 0;
    jint *c_array;
    jint arr_len;
    jboolean isCopy;
    c_array = (*env)->GetPrimitiveArrayCritical(env,j_array,&isCopy);
    printf("isCopy: %d \n", isCopy);
    if (c_array == NULL) {
        return 0;   // JVM复制原始数据到缓冲区失败
    }
    arr_len = (*env)->GetArrayLength(env,j_array);
    printf("arr_len = %d\n", arr_len);
    for (i = 0; i < arr_len; i++) {
        sum += c_array[i];
    }
    (*env)->ReleasePrimitiveArrayCritical(env, j_array, c_array, 0);
    return sum;
}
