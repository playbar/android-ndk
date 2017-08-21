package com.study.jnilearn;

public class JNIException {
	
	public static native void doit();
	
	public static void exceptionCallback() {
		int a = 20 / 0;
		System.out.println("--->" + a);
	}
	
	public static void normalCallback() {
		System.out.println("\nIn Java: invoke normalCallback.");
	}

	public static void main(String[] args) {
		doit();
	}
	
	static {
		System.loadLibrary("JNIException");
	}
}
