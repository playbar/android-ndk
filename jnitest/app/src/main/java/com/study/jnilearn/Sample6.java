package com.study.jnilearn;

import java.io.FileDescriptor;

/**
 * C代码启动虚拟机，加载该类，并调用main函数
 * @author yangxin
 *
 */
public class Sample6 {
	
	// 测试获取Java虚拟机指针变量和JNI_Env变量
	public native void getEnv();

	public static void main(String[] args) {
		System.out.println("Hello World " + args[0]);
		//Sample6 s = new Sample6();
		//s.getEnv();
	}

	static {
		System.loadLibrary("sample6");
	}
	
}
