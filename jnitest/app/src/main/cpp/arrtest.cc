
#include <jni.h>
#include <stddef.h>
#include "my_log.h"

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeArrTest(JNIEnv* env, jobject obj, jobjectArray objArr);

JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeIntSum(JNIEnv* env, jobject obj, jintArray intArr);

JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeStrSum(JNIEnv* env, jobject obj, jobjectArray objArr);

#ifdef __cplusplus
}
#endif

JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeArrTest(JNIEnv* env, jobject obj, jobjectArray objArr)
{
    int ilen = env->GetArrayLength(objArr);
    jclass objClass = env->FindClass("com/example/hellojni/Father");
    jfieldID id_age = env->GetFieldID(objClass, "age", "I");
    jfieldID id_num = env->GetFieldID(objClass, "num", "I");
    for(int i = 0; i < ilen; ++i) {
        jobject jobj = env->GetObjectArrayElement(objArr, i);
        int age = env->GetIntField(jobj, id_age);
        int num = env->GetIntField(jobj, id_num);
        LOGE("age=%d, num=%d", age, num);
    }
}

JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeIntSum(JNIEnv* env, jobject obj, jintArray array1)
{
    jint* arr;
    int sum=0;
    arr=env->GetIntArrayElements(array1,NULL);//得到一个指向原始数据类型内容的指针
    jint length=env->GetArrayLength(array1);//得到数组的长度
    int i=0;
    for(i=0;i<length;i++){
        sum+=arr[i];
    }
    sum = 0;

//第二种方法
    jint *buf = new jint[length];//定义一个jint类型的buffer把原始的数组copy到这个buf中去
    env->GetIntArrayRegion(array1,0,length,buf);

    for(i=0;i<length;i++){
        sum+=buf[i];
    }
    delete []buf;

//返回一个jint类型的数组

//可以先往一个数组中输入值，然后把这个数组copy到jintArray中
    jintArray iarr =env->NewIntArray(length);//新建一个jintArray
    env->SetIntArrayRegion(iarr, 0, length, buf);//将buf中的值复制到jintArray中去，数组copy

//打印新的数组值
    jint* arr2;
    arr2=env->GetIntArrayElements(iarr,NULL);
    for(i=0;i<env->GetArrayLength(iarr);i++){
        sum += arr2[i];
    }
//    env->ReleaseIntArrayElements()
    return;
}

JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeStrSum(JNIEnv* env, jobject obj, jobjectArray array2)
{
    int size=env->GetArrayLength(array2);//得到数组的长度值
    int i=0;
    for(i=0;i<size;i++){
        jstring obja=(jstring)env->GetObjectArrayElement(array2,i);
        const char* chars=env->GetStringUTFChars(obja,NULL);//将jstring类型转换成char类型输出
    }

//复制数组到新的数组中

    jclass objClass = env->FindClass("java/lang/String");//定义数组中元素类型
    jobjectArray texts= env->NewObjectArray(size, objClass, 0);//创建一个数组类型为String类型
    jstring jstr;
    for(i=0;i<size;i++)
    {
        jstr=(jstring)env->GetObjectArrayElement(array2,i);//把array2中的元素取出来
        env->SetObjectArrayElement(texts, i, jstr);//放入到texts数组中去，必须放入jstring
    }
    for(i=0;i<size;i++){
        jstring str1=(jstring)env->GetObjectArrayElement(texts,i);//打印出新复制的数组值
        const char* chars1=env->GetStringUTFChars(str1,NULL);
    }

    return;
}