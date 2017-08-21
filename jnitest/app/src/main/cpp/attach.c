
//attach.c
// 注意：这个程序只能运行在win32

#include <jni.h>
#include <string.h>

#ifdef _WIN32
	#define PATH_SEPARATOR ";"
#else
	#define PATH_SEPARATOR ":"
#endif

#ifdef _WIN32
    #include <Windows.h>
    #include <process.h>
#endif

#define USER_CLASSPATH "../bin/"

JavaVM *jvm;

void thread_fun(void *arg)
{
	jint status;
	jclass cls;
	jmethodID mid;
	jstring jstr;
	jclass stringClass;
	jobjectArray args;
	JNIEnv *env;
	char buff[100];
	int threadNum = (int)arg;
	memset(buff,0,sizeof(buff));
	// 每个线程分别挂载到当前进程的JVM中
	status = (*jvm)->AttachCurrentThread(jvm,(void**)&env, NULL);
	if (status != JNI_OK)
	{
		fprintf(stderr, "Attach Faild.\n");
		return;
	}
	cls = (*env)->FindClass(env,"com/study/jnilearn/Sample6");
	if (cls == NULL)
	{
		goto detach;
	}
	mid = (*env)->GetStaticMethodID(env,cls,"main","([Ljava/lang/String;)V");
	if (mid == NULL)
	{
		goto detach;
	}
	sprintf(buff,"from Thread %d",threadNum);
	jstr = (*env)->NewStringUTF(env,buff);
	if (jstr == NULL)
	{
		goto detach;
	}
	stringClass = (*env)->FindClass(env,"java/lang/String");
	if (stringClass == NULL)
	{
		goto detach;
	}
	args = (*env)->NewObjectArray(env,1,stringClass,jstr);
	if (args == NULL)
	{
		goto detach;
	}
	(*env)->CallStaticVoidMethod(env,cls,mid,args);

detach:
	if ((*env)->ExceptionOccurred(env))
	{
		(*env)->ExceptionDescribe(env);
	}
	// 将当前线程从JVM中卸载
	(*jvm)->DetachCurrentThread(jvm);
}

void main()
{
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	JavaVMOption options[2];
	jint status;
	int i;
#ifndef JNI_VERSION_1_2
	fprintf(stderr,"JDK Version Not 1.2 above.\n")
	exit(1)
#endif // !JNI_VERSION_1_2

	// 启动Java虚拟机的参数
	options[0].optionString = "-Djava.class.path=" USER_CLASSPATH;
	options[1].optionString = "-verbose:class";
	memset(&vm_args,0,sizeof(vm_args));
	// Java虚拟机初始化参数
	vm_args.options = options;
	vm_args.nOptions = 2;
	vm_args.version = JNI_VERSION_1_4;
	vm_args.ignoreUnrecognized = JNI_TRUE;
	// 启动Java虚拟机
	status = JNI_CreateJavaVM(&jvm,(void**)&env,&vm_args);
	if (status != JNI_OK)
	{
		fprintf(stderr,"Create Java VM Faild.\n");
		exit(1);
	}

	// 启动5个线程，每个线程分别挂载到当前进程当中正在运行的虚拟机中
	for (i = 0; i < 5; ++i)
	{
		_beginthread(thread_fun,0,(void*)i);
		//_beginthread(thread_fun, 0, (void *)i);
	}
	Sleep(1000);	// 等待子线程执行完毕

	(*jvm)->DestroyJavaVM(jvm);
	
}
