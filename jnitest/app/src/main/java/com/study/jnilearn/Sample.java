package com.study.jnilearn;

public class Sample {
	
	public native static String sayHello(String text);

	public static void main(String[] args) {
		String text = sayHello("yangxin");
		System.out.println("Java str: " + text);
	}
	
	static {
		System.loadLibrary("Sample");
	}
}
