
// ObjectArray.c

#include "com_study_jnilearn_ObjectArray.h"
/*
 * Class:     com_study_jnilearn_ObjectArray
 * Method:    initInt2DArray
 * Signature: (I)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_com_study_jnilearn_ObjectArray_initInt2DArray
  (JNIEnv *env, jobject obj, jint size)
{
    jobjectArray result;
    jclass clsIntArray;
    jint i,j;
    // 1.获得一个int型二维数组类的引用
    clsIntArray = (*env)->FindClass(env,"[I");
    if (clsIntArray == NULL)
    {
        return NULL;
    }
    // 2.创建一个数组对象（里面每个元素用clsIntArray表示）
    result = (*env)->NewObjectArray(env,size,clsIntArray,NULL);
    if (result == NULL)
    {
        return NULL;
    }
    
    // 3.为数组元素赋值
    for (i = 0; i < size; ++i)
    {
        jint buff[256];
        jintArray intArr = (*env)->NewIntArray(env,size);
        if (intArr == NULL)
        {
            return NULL;
        }
        for (j = 0; j < size; j++)
        {
            buff[j] = i + j;
        }
        (*env)->SetIntArrayRegion(env,intArr, 0,size,buff);
        (*env)->SetObjectArrayElement(env,result, i, intArr);
        (*env)->DeleteLocalRef(env,intArr);
    }
    
    return result;
}
