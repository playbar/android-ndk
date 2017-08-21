
// Sample6.c
// 因为JNIEnv指针不能跨线程使用，需要在用的时候才获取JNIEnv指针
//

#include "com_study_jnilearn_Sample6.h"

JavaVM *jvm;

/*
 * Class:     com_study_jnilearn_Sample6
 * Method:    getEnv
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample6_getEnv
  (JNIEnv *env1, jobject obj)
{
    // 第一种方式，将当前线程挂载到虚拟机中，从而从当前线程中获取JNIEnv指针
    JNIEnv *env2;
    if ((*jvm)->AttachCurrentThread(jvm,(void**)&env2, NULL) != JNI_OK)
    {
        // 将当前线程附加虚拟机中失败
    }
    
    printf("get JNIEnv Success.\n");
    
    if ((*jvm)->DetachCurrentThread(jvm) != JNI_OK) {
        // 卸载失败
    }
    
    JNIEnv *env3;
    
    // 第二种方式，JDK1.2以后提供了GETEnv接口，检查当前线程是否附加到了JVM上，然后返回属于当前线程的JNIEnv指针，如果当前线程已经被附加到了jvm上的话，和AttachCurrentThread获取JNIEnv指针的作用是一样的
    if((*jvm)->GetEnv(jvm, (void**)&env3,JNI_VERSION_1_4) != JNI_OK)
    {
        // 从虚拟机中获取JNIEnv指针失败
    }
    
    printf("env1==env2:%d\n", env1==env2);  // True
    printf("env1==env3:%d\n", env1==env3);  // True
    printf("env2==env3:%d\n", env2==env3);  // True
    
}

// 加载本地库之后的回调接口（操作系统调用）
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    // 第三种方式获取JNIEnv指针
    printf("call JNI_OnLoad\n");
    JNIEnv *env;
    if ((*vm)->GetEnv(vm,(void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        return JNI_ERR;
    }
    jvm = vm;   // 方式3：缓存JVM
    
    JavaVM *vm2;
    jsize nVms;
    if(JNI_GetCreatedJavaVMs(&vm2, 1, &nVms) != JNI_OK)
    {
        printf("get jvm faild.");
    }
    
    JavaVM *vm3;
    if((*env)->GetJavaVM(env,&vm3) != JNI_OK)
    {
        fprintf(stderr, "Get JVM Faild.");
    }
    
    printf("vm num:%d\n", nVms);    // 1
    printf("vm2==vm:%d\n", vm == vm2);  // True
    printf("vm==vm3:%d\n", vm == vm3);  // True
    printf("vm2==vm3:%d\n", vm2 == vm3);    // True
    return JNI_VERSION_1_4;
}

// 获取JAVA虚拟机
void JNU_GetJavaVM(JNIEnv* env)
{
    // 方式1，通过调用JNI函数获取
    if((*env)->GetJavaVM(env,&jvm) != JNI_OK)
    {
        fprintf(stderr, "Get JVM Faild.");
    }
    
    // 方式2：通过JNI_GetCreatedJavaVMs已创建的JAVA虚拟机中获取
    JavaVM *vm;
    jsize nVms;
    if(JNI_GetCreatedJavaVMs(&vm, 1, &nVms) != JNI_OK)
    {
        printf("get jvm faild.");
    }
}
