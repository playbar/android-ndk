
#include <jni.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_com_example_hellojni_HelloJni_nativeArrTest(JNIEnv* env, jobject obj, jintArray intArr, jobjectArray objArr);

#ifdef __cplusplus
}
#endif

JNIEXPORT void JNICALL
Java_com_example_hellojni_ArrTest_nativeArrTest(JNIEnv* env, jobject obj, jintArray intArr, jobjectArray objArr)
{
    jfieldID fid_arrays = env->GetFieldID(env->GetObjectClass(obj),"arrays","[I");
    jintArray jint_arr = (jintArray)env->GetObjectField(obj,fid_arrays);
    jint* int_arr = env->GetIntArrayElements(jint_arr,NULL);
    jsize len = env->GetArrayLength(jint_arr);

}