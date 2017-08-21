package com.study.jnilearn;

public class AccessCache {
	
	private String str = "Hello";
	
	public static native void initIDs(); 
	
	public native void accessField();
	
	public native String newString(char[] chars, int len);
	
	public native void nativeMethod();
	public void callback() {
		System.out.println("AccessCache.callback invoked!");
	}

	public static void main(String[] args) {
		AccessCache accessCache = new AccessCache();
		accessCache.nativeMethod();
		char chars[] = new char[7];
		chars[0] = '中';
		chars[1] = '华';
		chars[2] = '人';
		chars[3] = '民';
		chars[4] = '共';
		chars[5] = '和';
		chars[6] = '国';
		String str = accessCache.newString(chars, 6);
		System.out.println(str);
	}

	static {
		System.loadLibrary("AccessCache");
		initIDs();
	}
}
