package com.study.jnilearn;

public class AccessSuperMethod {
	
	public native static void callSuperInstanceMethod(); 

	public static void main(String[] args) {
		callSuperInstanceMethod();
	}
	
	static {
		System.loadLibrary("AccessSuperMethod");
	}

}
