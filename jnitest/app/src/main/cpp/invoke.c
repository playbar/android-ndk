#include <jni.h>
#include <stdlib.h>
#include <string.h>

/************************************************************************/
/* 1、编译（注意：需要jvm.lib的支持）
	cl -I"%JAVA_HOME%"\include -I"%JAVA_HOME%"\include\win32 invoke.c "%JAVA_HOME%"\lib\jvm.lib
   2、将jvm.dll设置到path路径下(因为运行时，需要加载jvm.dll动态库)
*/
/************************************************************************/

#ifdef _WIN32
	#define PATH_SEPARATOR ';'
#else
	#define PATH_SEPARATOR ':'
#endif

#define USER_CLASSPATH "../bin/"	// classpath当前路径

void main()
{
	JNIEnv *env;
	JavaVM *jvm;
	jint status;
	jint destory_status;
	jclass cls;
	jmethodID mid;
	jstring jstr;
	jclass stringClass;
	jobjectArray args;

#ifdef JNI_VERSION_1_2	// JDK1.2以上版本
	JavaVMInitArgs vm_args;		// 初始化JAVA虚拟机的参数
	JavaVMOption options[3];	// 启动Java虚拟的参数,相当于在命令行里传入的参数
	options[0].optionString = "-Djava.class.path=" USER_CLASSPATH;	// 设置虚拟机的类加载路径
	options[1].optionString = "-verbose:class";
	options[2].optionString = "-Xcheck:jni";
	memset(&vm_args,0,sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
	vm_args.nOptions = 3;	// options的大小
	vm_args.ignoreUnrecognized = JNI_TRUE;
	// 启动Java虚拟机
	status = JNI_CreateJavaVM(&jvm,(void**)&env,&vm_args);
#else	// 兼容JDK1.1版本
	JDK1_1InitArgs vm_args;
	char classpath[1024];
	vm_args.version = JNI_VERSION_1_1;
	JNI_GetDefaultJavaVMInitArgs(&vm_args);	// 获取JAVA虚拟机的默认参数
	// 追加用户自定义的classpath到默认的classpath路径下
	sprintf(classpath,"%s%c%s",vm_args.classpath, PATH_SEPARATOR, USER_CLASSPATH);
	vm_args.classpath = classpath;
	// 启动JAVA虚拟机
	status = JNI_CreateJavaVM(&jvm, &env, &vm_args);
#endif
	if (status != JNI_OK)
	{
		fprintf(stderr, "JVM Create failed!\n");
		exit(1);
	}

	// 找到Java程序的入口类
	cls = (*env)->FindClass(env,"com/study/jnilearn/Sample6");	
	if (cls == NULL)
	{
		goto destory;
	}
	// 找到JAVA程序入口类的入口函数
	mid = (*env)->GetStaticMethodID(env,cls,"main","([Ljava/lang/String;)V");
	if (mid == NULL)
	{
		goto destory;
	}

	jstr = (*env)->NewStringUTF(env,"from C!");
	if (jstr == NULL)
	{
		goto destory;	// 内存不足
	}

	stringClass = (*env)->FindClass(env,"java/lang/String");	// 从虚拟机中查找String类
	if (stringClass == NULL)
	{
		goto destory;
	}
	args = (*env)->NewObjectArray(env,1,stringClass,jstr);		// 设置main的入口参数值
	if (args == NULL)
	{
		goto destory;
	}

	(*env)->CallStaticVoidMethod(env,cls,mid,args);

destory:
	if ((*env)->ExceptionOccurred(env))
	{
		(*env)->ExceptionDescribe(env);	// 打印异常堆栈信息
	}
	destory_status = (*jvm)->DestroyJavaVM(jvm);	// 销毁Java虚拟机
	fprintf(stdout,"destory_status:%d\n",destory_status);
}
