
// Sample2.c

#include <stdio.h>
#include "com_study_jnilearn_Sample2.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_study_jnilearn_Sample2
 * Method:    instanceMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL  Java_com_study_jnilearn_Sample2_accessStaticField
  (JNIEnv *env, jobject obj)
{
	jfieldID fid;
	jint num;
	// 获取引用这个实例对象的class
	jclass cls = (*env)->GetObjectClass(env,obj);
	fid = (*env)->GetStaticFieldID(env,cls,"num","I");
	if (fid == NULL)
	{
		return;	// 找不到字段
	}

	printf("In C:\n");
	num = (*env)->GetStaticIntField(env,obj,fid);
	printf("Sample2.num:%d\n",num);
	// 设置静态字段的值
	(*env)->SetStaticIntField(env,cls,fid,200);
	
}

/*
 * Class:     com_study_jnilearn_Sample2
 * Method:    staticMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL  Java_com_study_jnilearn_Sample2_accessStaticMethod
  (JNIEnv *env, jclass cls)
{
	jmethodID mid;
	mid = (*env)->GetStaticMethodID(env,cls,"nativeMethod","(I)V");
	if (mid == NULL)
	{
		return;
	}

	(*env)->CallStaticVoidMethod(env,cls,mid,50);
}

/*
 * Class:     com_study_jnilearn_Sample2
 * Method:    accessPrimaryInstanceMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample2_accessPrimaryInstanceMethod
  (JNIEnv *env, jclass cls)
{
	// 创建Sample2的实例
	jmethodID mid;
	jobject obj;
    jfieldID fid;
	jint int_result;
	jfloat float_result;
	jboolean boolean_result;
	jdouble double_result;
	jchar char_result;
	jobject person_obj;
	jclass person_cls;
    jstring j_str;

	mid = (*env)->GetMethodID(env,cls,"<init>","()V");
	if (mid == NULL)
	{
		return;	// 没有无参的构造函数
	}

	obj = (*env)->NewObject(env,cls,mid);
	if (obj == NULL)
	{
		return;	// 内存不足，创建对象失败
	}

	// 调用Sample2实例的相关实例函数
	// 1.带int参数int返回值的函数
	mid = (*env)->GetMethodID(env,cls,"intMethod","(I)I");
	if (mid == NULL)
	{
		return;	// 找不到int intMethod(int)这个函数
	}
	
	int_result = (*env)->CallIntMethod(env,obj,mid,10);
	printf("In C Call intMethod:%d\n", int_result);

	// 2.带float参数float返回值的函数
	mid = (*env)->GetMethodID(env,cls,"floatMethod","(F)F");
	if (mid == NULL)
	{
		return;
	}
	float_result = (*env)->CallFloatMethod(env,obj,mid,10.5f);
	printf("In C Call floatMethod:%.2f\n",float_result);

	// 3.double参数和返回值函数
	mid = (*env)->GetMethodID(env,cls,"doubleMethod","(D)D");
	if (mid == NULL)
	{
		return;
	}
	double_result = (*env)->CallDoubleMethod(env,obj,mid,15.5);
	printf("In C Call doubleMethod:%.2f\n",double_result);

	// 4.booleal参数和返回值函数
	mid = (*env)->GetMethodID(env,cls,"booleanMethod","(Z)Z");
	if (mid == NULL)
	{
		return;
	}
	boolean_result = (*env)->CallBooleanMethod(env,obj,mid,1);
	printf("In C Call booleanMethod:%d\n",boolean_result);


	// 5.char参数和返回值函数
	mid = (*env)->GetMethodID(env,cls,"charMethod","(C)C");
	if (mid == NULL)
	{
		return;
	}
	char_result = (*env)->CallCharMethod(env,obj,mid,'C');
	printf("In C Call charMethod:%c\n",char_result);

	// 6.java对象boolean返回值函数
	person_cls = (*env)->FindClass(env,"com/study/jnilearn/Person");	// 和方法参数中引用类型的写不支不同，只需要将.换成/就行
	if (cls == NULL)
	{
		return;	// 在虚拟机中没有找到com.study.jnilearn.Person这个类
	}
	mid = (*env)->GetMethodID(env,person_cls,"<init>","()V");
	if (mid == NULL)
	{
		return;	// 没有找到构造函数
	}
	person_obj = (*env)->NewObject(env,person_cls,mid);
    if (person_obj == NULL) {
        return; // 创建对象失败，内存不足
    }
    
    // 封装数据
    fid = (*env)->GetFieldID(env,person_cls,"name","Ljava/lang/String;");   // 找到name字段
    if (fid == NULL) {
        return; // 没有找到name这个字段
    }
    j_str = (*env)->NewStringUTF(env,"刘备"); // 创建一个unicode字符串
    if (j_str == NULL) {
        return; // 内存不足
    }
    (*env)->SetObjectField(env,person_obj,fid,j_str);   // 设置对Person对象name字段的值
    
    mid = (*env)->GetMethodID(env,cls,"queryPerson","(Lcom/study/jnilearn/Person;)Z");  // 获取Person类的Class对象
    if (mid == NULL)
    {
        return;	// 没有找到boolean queryPerson(com.study.jnilearn.Person p)这个方法
    }
	boolean_result = (*env)->CallBooleanMethod(env,obj,mid,person_obj); // 调用queryPerson方法，并将jni中创建的person对象传进去
    printf("%s person",boolean_result ? "find" : "not find");
}


#ifdef __cplusplus
}
#endif
