
// Sample5.c

#include "com_study_jnilearn_Sample5.h"
#include "jnu.h"

/*
 * Class:     com_study_jnilearn_Sample5
 * Method:    doit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample5_doit
  (JNIEnv *env, jobject obj)
{
    jthrowable exec = NULL;
    jclass cls;
    jmethodID mid;
    cls = (*env)->GetObjectClass(env,obj);
    /*mid = (*env)->GetMethodID(env,cls,"callback","()V");
    if (mid == NULL) {
        return;
    }
    (*env)->CallVoidMethod(env,obj,mid);*/    // 调用callback函数
    
    JNU_CallMethodByName(env, NULL, obj, "callback", "()V");
    
    // 检查Java层是否发生了异常，并返回发生的异常类对象
    exec = (*env)->ExceptionOccurred(env);
    if (exec)
    {
        (*env)->ExceptionDescribe(env); // 打印Java的异常堆栈信息
        (*env)->ExceptionClear(env);    // 清除异常，执行自己的异常处理逻辑
        
        // 打印这个异常的信息
        mid = (*env)->GetMethodID(env,cls,"printExcepiton","(Ljava/lang/Exception;)V");
        if (mid == NULL) {
            return;
        }
        (*env)->CallVoidMethod(env,obj,mid,exec);
        
        (*env)->DeleteLocalRef(env,exec);
        
        // 抛出一个异常
        //jclass new_exec;
        /*new_exec = (*env)->FindClass(env,"java/lang/IllegalArgumentException");
        if (new_exec == NULL)
        {
            return;
        }
        
        (*env)->ThrowNew(env,new_exec,"throw from C Code");
        (*env)->DeleteLocalRef(env,new_exec);*/
        JNU_ThrowByName(env, "java/lang/IllegalArgumentException", "throw from C Code");
    }
    (*env)->DeleteLocalRef(env,cls);
}


/*
 * Class:     com_study_jnilearn_Sample5
 * Method:    errorCodeCheck
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample5_errorCodeCheck
(JNIEnv *env, jobject obj)
{
    jmethodID mid;
    jclass cls;
    jint result;
    cls = (*env)->GetObjectClass(env,obj);
    mid = (*env)->GetMethodID(env,cls,"floor","()I");
    if (mid == NULL) {
        return;
    }
    result = (*env)->CallIntMethod(env,obj,mid);
    printf("In C result:%d\n",result);
    
    // 检查异常
    if ((*env)->ExceptionCheck(env))
    {
        // 必须先检查、处理、清除异常后再做其它 JNI 函数调用,否 则的话,结果未知
        printf("c check error throw.....\n");
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return;
    }

}
