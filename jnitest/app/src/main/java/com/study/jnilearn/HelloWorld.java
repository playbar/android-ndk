package com.study.jnilearn;


public class HelloWorld {

	class MyClass {}

	public static native void test(short s, int i, long l, float f, double d, char c, 
			boolean z, byte b, String str, Object obj, MyClass p, int[] arr);

//	public static void main(String[] args) {
//		String obj = "obj";
//		short s = 1;
//		long l = 20;
//		byte b = 100;
//		test(s, 1, l, 1.0f, 10.5, 'A', true, b, "中国", obj, new MyClass(), new int[] {});
//	}
	
	static {
		System.loadLibrary("HelloWorld");
	}
}
