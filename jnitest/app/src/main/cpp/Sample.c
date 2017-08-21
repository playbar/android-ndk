// Sample.c
#include "com_study_jnilearn_Sample.h"
/*
 * Class:     com_study_jnilearn_Sample
 * Method:    sayHello
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_study_jnilearn_Sample_sayHello
  (JNIEnv *env, jclass cls, jstring j_str)
{
	const char *c_str = NULL;
	char buff[128] = {0};
	c_str = (*env)->GetStringUTFChars(env, j_str, NULL);
	if(c_str == NULL)
	{
		return NULL;
	}
	printf("C_str: %s \n", c_str);
	sprintf(buff, "hello %s", c_str);
	(*env)->ReleaseStringUTFChars(env, j_str, c_str);
	return (*env)->NewStringUTF(env,buff);
}
