package com.study.jnilearn;

/**
 * 本地代码访问类的实例方法和静态方法
 * @author yangxin
 */
public class AccessMethod {
	
	public static native void callJavaStaticMethod(); 
	public static native void callJavaInstaceMethod();
	
	public static void main(String[] args) {
		callJavaStaticMethod();
		callJavaInstaceMethod();
	}
	
	static {
		System.loadLibrary("AccessMethod");
	}
}
