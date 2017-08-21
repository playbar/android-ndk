
// Sample7.c 综合案例

#include <jni.h>
#include "jnu.h"
#include <string.h>
#include <stdlib.h>

#define CLASS_NAME "com/study/jnilearn/Sample7"

static JavaVM *g_jvm;
static jclass g_sample_class;
static jclass g_person_class;

static jfieldID FID_Sample7_persons;


void /*JNICALL*/ initIDs(JNIEnv *env, jclass cls)
{
    FID_Sample7_persons = (*env)->GetFieldID(env,cls,"persons","[Lcom/study/jnilearn/Person;");
}

jobject /*JNICALL*/ queryPerson(JNIEnv *env, jclass cls, jstring jstr)
{
    jobject result = NULL;
    jobject sample7;
    jobjectArray jperson_arr;
    jobject jperson;
    jint len;
    jfieldID fid_name;
    jstring jstr_name;
    jmethodID mid_sample;
    const char *c_name;
    const char *c_item_name;
    jboolean isFind = JNI_FALSE;
    int i;
    c_name = (*env)->GetStringUTFChars(env,jstr,NULL);
    if (c_name == NULL)
    {
        return NULL;
    }
    printf("find in %s.....\n",c_name);
    mid_sample = (*env)->GetMethodID(env,cls, "<init>", "()V");
    if ((*env)->ExceptionCheck(env))
    {
        goto release_res;
    }
    sample7 = (*env)->AllocObject(env,cls); // 分配内存
    if (sample7)
    {
        (*env)->CallNonvirtualVoidMethod(env,sample7,cls,mid_sample);   // 调用默认的构造函数初始化对象
        if ((*env)->ExceptionCheck(env))
        {
            goto release_res;
        }
    }
    /*
    sample7 = (*env)->NewObject(env,cls, mid_sample);
    if ((*env)->ExceptionCheck(env))
    {
        return NULL;
    }*/
    
    fid_name = (*env)->GetFieldID(env, g_person_class, "name", "Ljava/lang/String;");
    if ((*env)->ExceptionCheck(env))
    {
        goto release_res;
    }
    jperson_arr = (*env)->GetObjectField(env,sample7,FID_Sample7_persons);
    if ((*env)->ExceptionCheck(env))
    {
        goto release_res;
    }
    len = (*env)->GetArrayLength(env,jperson_arr);  // 获取数组的长度
    for (i = 0; i < len; i++)
    {
        jperson = (*env)->GetObjectArrayElement(env,jperson_arr, i);
        jstr_name = (jstring)(*env)->GetObjectField(env,jperson,fid_name);
        if (jstr_name == NULL)
        {
            goto release_res;
        }
        c_item_name = (*env)->GetStringUTFChars(env,jstr_name, NULL);
        printf("%s--->%s\n",c_item_name, c_name);
        if (strcmp(c_item_name,c_name) == 0)
        {
            printf("%s is find!\n", c_name);
            result = (*env)->NewWeakGlobalRef(env,jperson);
            // result = jperson; // error JVM会回收该对象
            isFind = JNI_TRUE;
        }
        // 释放资源（避免循环比较长，每循环一次都创建一个局部引用，避免造成引用表溢出）
        (*env)->ReleaseStringUTFChars(env,jstr_name,c_item_name);
        (*env)->DeleteLocalRef(env,jstr_name);
        (*env)->DeleteLocalRef(env,jperson);
        if (isFind)
        {
            printf("find finish...\n");
            break;
        }
    }
    
    if (!result) {
        printf("can't find %s!\n", c_name);
    }
    
release_res:
    (*env)->ReleaseStringUTFChars(env,jstr,c_name);
    // 如果引用为NULL，DeletexxxRef不会报错
    (*env)->DeleteLocalRef(env,sample7);
    (*env)->DeleteLocalRef(env,jperson_arr);
    
    return result;
}

void /*JNICALL*/ printInfo(JNIEnv *env, jclass cls, jobject obj)
{
    if (obj == NULL)
    {
        printf("Person is NULL, can't print Person inf!\n");
        return;
    }
    jstring jstr_name;
    jclass personClass;
    jfieldID fid_name, fid_age, fid_weight, fid_height, fid_sex;
    const char* c_name;
    jint c_age;
    jfloat c_weight;
    jdouble c_height;
    jchar c_sex;
    personClass = (*env)->GetObjectClass(env,obj);
    if ((*env)->ExceptionCheck(env))
    {
        return;
    }
    fid_name = (*env)->GetFieldID(env,personClass,"name","Ljava/lang/String;");
    fid_age = (*env)->GetFieldID(env, personClass, "age", "I");
    fid_weight = (*env)->GetFieldID(env, personClass, "weight", "F");
    fid_height = (*env)->GetFieldID(env, personClass, "height", "D");
    fid_sex = (*env)->GetFieldID(env, personClass, "sex", "C");
    if ((*env)->ExceptionCheck(env))
    {
        printf("find field error!\n");
        return;
    }
    jstr_name = (*env)->GetObjectField(env,obj,fid_name);
    c_age = (*env)->GetIntField(env, obj, fid_age);
    c_weight = (*env)->GetFloatField(env, obj, fid_weight);
    c_height = (*env)->GetDoubleField(env, obj, fid_height);
    c_sex = (*env)->GetCharField(env, obj, fid_sex);
    if ((*env)->ExceptionCheck(env))
    {
        return;
    }
    c_name = (*env)->GetStringUTFChars(env,jstr_name,NULL);
    if (c_name == NULL)
    {
        (*env)->DeleteLocalRef(env,jstr_name);
        return;
    }
    printf("name:%s\n", c_name);
    printf("age:%d\n", c_age);
    printf("weight:%.1f\n", c_weight);
    printf("height:%.1lf\n", c_height);
    printf("sex:%u\n", c_sex);
    
    (*env)->ReleaseStringUTFChars(env,jstr_name, c_name);
    (*env)->DeleteLocalRef(env,jstr_name);
}

jint /*JNICALL*/ getAvgage(JNIEnv *env, jobject thiz)
{
    jint sum = 0;
    jobjectArray jperson_arr;
    jobject jperson;
    jfieldID fid_age;
    jint len;
    int i;
    jperson_arr =(*env)->GetObjectField(env,thiz,FID_Sample7_persons);
    if ((*env)->ExceptionCheck(env))
    {
        return 0;
    }
    len = (*env)->GetArrayLength(env,jperson_arr);
    fid_age = (*env)->GetFieldID(env,g_person_class,"age","I");
    if ((*env)->ExceptionCheck(env)) {
        return 0;
    }
    
    for (i = 0; i < len; i++)
    {
        jperson = (*env)->GetObjectArrayElement(env,jperson_arr,i);
        sum = sum + (*env)->GetIntField(env, jperson, fid_age);
        (*env)->DeleteLocalRef(env,jperson);
    }
    (*env)->DeleteLocalRef(env,jperson_arr);
    return sum / len;;
}

jstring /*JNICALL*/ getNames(JNIEnv *env, jobject thiz)
{
    jstring resutl = NULL;
    jobjectArray j_persons;
    jobject j_person;
    jfieldID fid_name;
    jint len;
    char *buff;
    jstring jstr_name;
    const char *c_name;
    int i;
    
    j_persons = (jobjectArray)(*env)->GetObjectField(env,thiz,FID_Sample7_persons);
    if ((*env)->ExceptionCheck(env))
    {
        return NULL;
    }
   
    len = (*env)->GetArrayLength(env,j_persons);
    fid_name = (*env)->GetFieldID(env,g_person_class,"name","Ljava/lang/String;");
    if ((*env)->ExceptionCheck(env))
    {
        (*env)->DeleteLocalRef(env,j_persons);
        return NULL;
    }
    
    buff = (char*)malloc(4096);
    memset((void*)buff, 0, 4096);
    for (i = 0; i < len; i++)
    {
        j_person = (*env)->GetObjectArrayElement(env,j_persons,i);
        jstr_name = (*env)->GetObjectField(env,j_person,fid_name);
        c_name = (*env)->GetStringUTFChars(env,jstr_name, NULL);
        if ((*env)->ExceptionCheck(env))
        {
            // 释放从虚拟机中所获取的字符串分配的内存
            (*env)->ReleaseStringUTFChars(env,jstr_name,c_name);
            return NULL;
        }
        sprintf(buff, "%s%s,",buff,c_name);
        // 释放从虚拟机中所获取的字符串分配的内存
        (*env)->ReleaseStringUTFChars(env,jstr_name,c_name);
        // 手动删除局部引用
        (*env)->DeleteLocalRef(env,j_person);
        (*env)->DeleteLocalRef(env,jstr_name);
    }
    
    (*env)->DeleteLocalRef(env,j_persons);
    resutl = (*env)->NewStringUTF(env,buff);
    free(buff);
    return resutl;
}

const JNINativeMethod g_methods[] = {
    {"initIDs", "()V", (void*)initIDs},
    {"queryPerson", "(Ljava/lang/String;)Lcom/study/jnilearn/Person;", (void*)queryPerson},
    {"printInfo", "(Lcom/study/jnilearn/Person;)V", (void*)printInfo},
    {"getAvgage", "()I", (void*)getAvgage},
    {"getNames", "()Ljava/lang/String;", (void*)getNames}
    
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    printf("JNI_OnLoad\n");
    JNIEnv* env;
    jclass sample7Class;
    jclass personClass;
    jint reg_res = -1;
    if ((*vm)->GetEnv(vm,(void**)&env,JNI_VERSION_1_4) != JNI_OK)
    {
        return JNI_ERR; // 不支持这个版本的JNI
    }
    g_jvm = vm;   // 缓存JAVA虚拟机
    sample7Class = (*env)->FindClass(env,CLASS_NAME);
    personClass = (*env)->FindClass(env,"com/study/jnilearn/Person");
    if ((*env)->ExceptionCheck(env))
    {
        return JNI_ERR;
    }
    g_sample_class = (*env)->NewWeakGlobalRef(env,sample7Class);
    g_person_class = (*env)->NewWeakGlobalRef(env,personClass);
    (*env)->DeleteLocalRef(env,sample7Class);
    (*env)->DeleteLocalRef(env,personClass);
    // 注册本地方法
    reg_res = (*env)->RegisterNatives(env,g_sample_class,g_methods,sizeof(g_methods) / sizeof(g_methods[0]));
    if (reg_res != JNI_OK)
    {
        return JNI_ERR;
    }
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    printf("JNI_OnUnload\n");
    JNIEnv* env;
    if ((*vm)->GetEnv(vm,(void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        return;
    }
    (*env)->UnregisterNatives(env,g_sample_class);
    (*env)->DeleteWeakGlobalRef(env,g_sample_class);
    (*env)->DeleteWeakGlobalRef(env,g_person_class);
}

