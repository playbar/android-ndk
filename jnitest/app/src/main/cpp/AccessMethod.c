// AccessMethod.c

#include "com_study_jnilearn_AccessMethod.h"

/*
 * Class:     com_study_jnilearn_AccessMethod
 * Method:    callJavaStaticMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessMethod_callJavaStaticMethod
(JNIEnv *env, jclass cls)
{
    jclass clazz = NULL;
    jstring str_arg = NULL;
    jmethodID mid_static_method;
    // 1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    clazz = (*env)->FindClass(env,"com/study/jnilearn/ClassMethod");
    if (clazz == NULL) {
        return;
    }
    
    // 2、从clazz类中查找callStaticMethod方法
    mid_static_method = (*env)->GetStaticMethodID(env,clazz,"callStaticMethod","(Ljava/lang/String;I)V");
    if (mid_static_method == NULL) {
        printf("找不到callStaticMethod这个静态方法。");
        return;
    }
    
    // 3、调用clazz类的callStaticMethod静态方法
    str_arg = (*env)->NewStringUTF(env,"我是静态方法");
    (*env)->CallStaticVoidMethod(env,clazz,mid_static_method, str_arg, 100);
    
    // 删除局部引用
    (*env)->DeleteLocalRef(env,clazz);
    (*env)->DeleteLocalRef(env,str_arg);
}

/*
 * Class:     com_study_jnilearn_AccessMethod
 * Method:    callJavaInstaceMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessMethod_callJavaInstaceMethod
(JNIEnv *env, jclass cls)
{
    jclass clazz = NULL;
    jobject jobj = NULL;
    jmethodID mid_construct = NULL;
    jmethodID mid_instance = NULL;
    jstring str_arg = NULL;
    // 1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    clazz = (*env)->FindClass(env, "com/study/jnilearn/ClassMethod");
    if (clazz == NULL) {
        printf("找不到'com.study.jnilearn.ClassMethod'这个类");
        return;
    }
    
    // 2、获取类的默认构造方法ID
    mid_construct = (*env)->GetMethodID(env,clazz, "<init>","()V");
    if (mid_construct == NULL) {
        printf("找不到默认的构造方法");
        return;
    }
    
    // 3、查找实例方法的ID
    mid_instance = (*env)->GetMethodID(env, clazz, "callInstanceMethod", "(Ljava/lang/String;I)V");
    if (mid_instance == NULL) {
        
        return;
    }
    
    // 4、创建该类的实例
    jobj = (*env)->NewObject(env,clazz,mid_construct);
    if (jobj == NULL) {
        printf("在com.study.jnilearn.ClassMethod类中找不到callInstanceMethod方法");
        return;
    }
    
    // 5、调用对象的实例方法
    str_arg = (*env)->NewStringUTF(env,"我是实例方法");
    (*env)->CallVoidMethod(env,jobj,mid_instance,str_arg,200);
    
    // 删除局部引用
    (*env)->DeleteLocalRef(env,clazz);
    (*env)->DeleteLocalRef(env,jobj);
    (*env)->DeleteLocalRef(env,str_arg);
}


