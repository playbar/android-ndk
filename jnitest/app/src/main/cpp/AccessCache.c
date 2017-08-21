// AccessCache.c

#include "com_study_jnilearn_AccessCache.h"

JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessCache_accessField
  (JNIEnv *env, jobject obj)
{
    // 第一次访问时将字段放到内存数据区，直到程序结束才会释放，可以起到缓存的作用
    static jfieldID fid_str = NULL;
    jclass cls_AccessCache;
    jstring j_str;
    const char *c_str;
    cls_AccessCache = (*env)->GetObjectClass(env, obj); // 获取该对象的Class引用
    if (cls_AccessCache == NULL) {
        return;
    }
    
    // 先判断字段ID之前是否已经缓存过，如果已经缓存过则不进行查找
    if (fid_str == NULL) {
        fid_str = (*env)->GetFieldID(env,cls_AccessCache,"str","Ljava/lang/String;");
        
        // 再次判断是否找到该类的str字段
        if (fid_str == NULL) {
            return;
        }
    }
    
    j_str = (*env)->GetObjectField(env, obj, fid_str);  // 获取字段的值
    c_str = (*env)->GetStringUTFChars(env, j_str, NULL);
    if (c_str == NULL) {
        return; // 内存不够
    }
    printf("In C:\n str = \"%s\"\n", c_str);
    (*env)->ReleaseStringUTFChars(env, j_str, c_str);   // 释放从从JVM新分配字符串的内存空间
    
    // 修改字段的值
    j_str = (*env)->NewStringUTF(env, "12345");
    if (j_str == NULL) {
        return;
    }
    (*env)->SetObjectField(env, obj, fid_str, j_str);
    
    // 释放本地引用
    (*env)->DeleteLocalRef(env,cls_AccessCache);
    (*env)->DeleteLocalRef(env,j_str);
}


JNIEXPORT jstring JNICALL Java_com_study_jnilearn_AccessCache_newString
(JNIEnv *env, jobject obj, jcharArray j_char_arr, jint len)
{
    jcharArray elemArray;
    jchar *chars = NULL;
    jstring j_str = NULL;
    static jclass cls_string = NULL;
    static jmethodID cid_string = NULL;
    // 缓存String的class引用
    if (cls_string == NULL) {
        cls_string = (*env)->FindClass(env, "java/lang/String");
        if (cls_string == NULL) {
            return NULL;
        }
    }
    
    // 缓存String的构造方法ID
    if (cid_string == NULL) {
        cid_string = (*env)->GetMethodID(env, cls_string, "<init>", "([C)V");
        if (cid_string == NULL) {
            return NULL;
        }
    }
    
    printf("In C array Len: %d\n", len);
    // 创建一个字符数组
    elemArray = (*env)->NewCharArray(env, len);
    if (elemArray == NULL) {
        return NULL;
    }
    
    // 获取数组的指针引用，注意：不能直接将jcharArray作为SetCharArrayRegion函数最后一个参数
    chars = (*env)->GetCharArrayElements(env, j_char_arr,NULL);
    if (chars == NULL) {
        return NULL;
    }
    // 将Java字符数组中的内容复制指定长度到新的字符数组中
    (*env)->SetCharArrayRegion(env, elemArray, 0, len, chars);
    
    // 调用String对象的构造方法，创建一个指定字符数组为内容的String对象
    j_str = (*env)->NewObject(env, cls_string, cid_string, elemArray);
    
    // 释放本地引用
    (*env)->DeleteLocalRef(env, elemArray);
    
    return j_str;

}

jmethodID MID_AccessCache_callback;

JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessCache_initIDs
(JNIEnv *env, jclass cls)
{
    printf("initIDs called!!!\n");
    MID_AccessCache_callback = (*env)->GetMethodID(env,cls,"callback","()V");
}

JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessCache_nativeMethod
(JNIEnv *env, jobject obj)
{
    printf("In C Java_com_study_jnilearn_AccessCache_nativeMethod called!!!\n");
    (*env)->CallVoidMethod(env, obj, MID_AccessCache_callback);
}

