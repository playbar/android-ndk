
// thread_sync.c 线程同步
#include "com_study_jnilearn_ThreadSync.h"

// 预先缓存Object类线程通信的方法ID
static jmethodID MID_Object_wait;
static jmethodID MID_Object_notify;
static jmethodID MID_Object_notifyAll;

JNIEXPORT void JNICALL Java_com_study_jnilearn_ThreadSync_initIDs
  (JNIEnv *env, jclass cls)
{
    // 每个对象的父类都是Object的子类
    MID_Object_wait = (*env)->GetMethodID(env,cls,"wait","(J)V");
    if (MID_Object_wait == NULL)
    {
        return;
    }
    
    MID_Object_notify = (*env)->GetMethodID(env,cls, "notify", "()V");
    if (MID_Object_notify == NULL)
    {
        return;
    }
    
    MID_Object_notifyAll = (*env)->GetMethodID(env,cls, "notifyAll", "()V");
    if (MID_Object_notifyAll == NULL)
    {
        return;
    }
}

// 调用对象的wait方法
void JNU_MonitorWait(JNIEnv *env, jobject obj, jlong timeout)
{
    (*env)->CallVoidMethod(env,obj,MID_Object_wait,timeout);
}

// 调用对象的notify方法
void JNU_MonitorNotify(JNIEnv *env, jobject obj)
{
    (*env)->CallVoidMethod(env,obj,MID_Object_notify);
}

// 调用对象的notifyAll方法
void JNU_MonitorNotifyAll(JNIEnv *env, jobject obj)
{
    (*env)->CallVoidMethod(env, obj, MID_Object_notifyAll);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env;
    jobject obj;
    if ((*vm)->GetEnv(vm, (void**)&env,JNI_VERSION_1_4) != JNI_OK)
    {
        return JNI_ERR;
    }
    
    // 线程同步
    if((*env)->MonitorEnter(env,obj) != JNI_OK) // 上锁，成功返回JNI_OK
    {
        // ....同步异常处理
    }
    
    if ((*env)->ExceptionOccurred(env)) {
        // 处理异常
        (*env)->ExceptionDescribe(env);
        
        if ((*env)->MonitorExit(env,obj) != JNI_OK)
        {
            // 处理退出同步块的异常
        }
    }
    
    // 执行同步代码块之间的代码。。。。
    
    if ((*env)->MonitorExit(env,obj) != JNI_OK) // 解锁，成功返回JNI_OK
    {
        // ... 同步异常处理
    }
    
    /* JNI没有提供Java Object类的wait, notify,notifyAll方法，所以在写线程同步的时候，建议通过JNI函数调用JAVA提供
     的线程同步接口
     */
    
    return JNI_VERSION_1_4;
}
