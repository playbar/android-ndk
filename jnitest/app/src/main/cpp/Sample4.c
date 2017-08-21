// Sample4.c 局部引用、全局引用、弱全局引用

#include "com_study_jnilearn_Sample4.h"
#include <unistd.h>

/*
 * Class:     com_study_jnilearn_Sample4
 * Method:    newString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_study_jnilearn_Sample4_newString
  (JNIEnv *env, jobject obj, jcharArray chars, jint len)
{
	static jclass stringClass = NULL;
	jclass localClsRef = NULL;
	jmethodID mid = NULL;
	jcharArray elemArr = NULL;
	jstring result = NULL;
	if (stringClass == NULL)
	{
		printf("global ref is null");
		localClsRef = (*env)->FindClass(env,"java/lang/String");
		if (localClsRef == NULL)
		{
			return NULL;
		}

		// 创建一个全局引用
		stringClass = (*env)->NewGlobalRef(env,localClsRef);

		// 删除已使用的局部引用，避免阻止GC回收对象，并避免造成占用的函数表的位置及内存
		(*env)->DeleteLocalRef(env,localClsRef);

		// 再次校验全局引用是否创建成功
		if (stringClass == NULL)
		{
			return NULL;
		}
	}

	//(*env)->GetMethodID(env,stringClass,"<init>","()V");	// 无参构造函数
	mid = (*env)->GetMethodID(env,stringClass,"<init>","([C)V");
	if (mid == NULL)
	{
		return NULL;	// 未找到构造函数
	}

	/*elemArr = (*env)->NewCharArray(env,len);
	if (elemArr == NULL)
	{
		return NULL;
	}

	(*env)->SetCharArrayRegion(env,elemArr,0,len,(const jchar*)chars);*/

	// 调用String的构造方法，创建对象
	result = (jstring)(*env)->NewObject(env,stringClass,mid,chars);

	// 删除局部引用
	(*env)->DeleteLocalRef(env,elemArr);
	return result;
}

/*
 * Class:     com_study_jnilearn_Sample4
 * Method:    testRefresh
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_study_jnilearn_Sample4_testRefresh
  (JNIEnv *env, jobject obj, jint len)
{
    jobject obj2;
    jstring str;
    jboolean isSample;
    obj2 = obj;
    isSample = (*env)->IsSameObject(env,obj,obj2);
    printf("(*env)->IsSameObject(env,obj,obj2): %d\n",isSample);
    obj2 = NULL;
    isSample = (*env)->IsSameObject(env,obj2,NULL);
    printf("(*env)->IsSameObject(env,obj2,NULL): %d\n",isSample);
    
    str = (*env)->NewStringUTF(env,"NewString");
    obj2 = (*env)->NewWeakGlobalRef(env,str);
    
	int i;
	for (i = 0; i < len; i++)
	{
        jstring str = (*env)->NewStringUTF(env,"hello");
        printf("new string:%d\n",i+1);
	}
    
    isSample = (*env)->IsSameObject(env,str,obj2);
    printf("(*env)->IsSameObject(env,str,obj2):%d\n",isSample);
    //obj2 = NULL;
    sleep(5);
    isSample = (*env)->IsSameObject(env,obj2,NULL); // 如果弱引用被gc回收，则返回TRUE，否则返回FALSE
    printf("(*env)->IsSameObject(env,obj2,NULL):%d\n",isSample);

}
