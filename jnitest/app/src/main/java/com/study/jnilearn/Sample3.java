package com.study.jnilearn;

/**
 * 缓存字段和方法ID
 * @author yangxin
 *
 */
public class Sample3 {

	private String str = "Cocos2d-x";
	
	private native void accessInstanceField(); 
	private native void nativeMethod();
	
	private native static void initIds();
	
	private void callback() {
		System.out.println("java Call back");
	}
	
	public static void main(String[] args) {
		Sample3 s = new Sample3();
		s.accessInstanceField();
		System.out.println("in Java:" + s.str);
		
		s.nativeMethod();
	}
	
	static {
		System.loadLibrary("sample3");
		initIds();
	}
	

}
