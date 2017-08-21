// AccessField.c

#include "com_study_jnilearn_AccessField.h"

/*
 * Class:     com_study_jnilearn_AccessField
 * Method:    accessInstanceField
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessField_accessInstanceField
(JNIEnv *env, jclass cls, jobject obj)
{
    jclass clazz;
    jfieldID fid;
    jstring j_str;
    jstring j_newStr;
    const char *c_str = NULL;
    
    // 1.获取AccessField类的Class引用
    clazz = (*env)->GetObjectClass(env,obj);
    if (clazz == NULL) {
        return;
    }
    
    // 2. 获取AccessField类实例变量str的属性ID
    fid = (*env)->GetFieldID(env,clazz,"str", "Ljava/lang/String;");
    if (clazz == NULL) {
        return;
    }
    
    // 3. 获取实例变量str的值
    j_str = (jstring)(*env)->GetObjectField(env,obj,fid);
    
    // 4. 将unicode编码的java字符串转换成C风格字符串
    c_str = (*env)->GetStringUTFChars(env,j_str,NULL);
    if (c_str == NULL) {
        return;
    }
    printf("In C--->ClassField.str = %s\n", c_str);
    (*env)->ReleaseStringUTFChars(env, j_str, c_str);
    
    // 5. 修改实例变量str的值
    j_newStr = (*env)->NewStringUTF(env, "This is C String");
    if (j_newStr == NULL) {
        return;
    }
    
    (*env)->SetObjectField(env, obj, fid, j_newStr);
    
    // 6.删除局部引用
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, j_str);
    (*env)->DeleteLocalRef(env, j_newStr);
}

/*
 * Class:     com_study_jnilearn_AccessField
 * Method:    accessStaticField
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_AccessField_accessStaticField
(JNIEnv *env, jclass cls)
{
    jclass clazz;
    jfieldID fid;
    jint num;
    
    //1.获取ClassField类的Class引用
    clazz = (*env)->FindClass(env,"com/study/jnilearn/ClassField");
    if (clazz == NULL) {    // 错误处理
        return;
    }
    
    //2.获取ClassField类静态变量num的属性ID
    fid = (*env)->GetStaticFieldID(env, clazz, "num", "I");
    if (fid == NULL) {
        return;
    }
    
    // 3.获取静态变量num的值
    num = (*env)->GetStaticIntField(env,clazz,fid);
    printf("In C--->ClassField.num = %d\n", num);
    
    // 4.修改静态变量num的值
    (*env)->SetStaticIntField(env, clazz, fid, 80);
    
    // 删除属部引用
    (*env)->DeleteLocalRef(env,clazz);
}
