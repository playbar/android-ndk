package com.study.jnilearn;

/**
 * ClassMethod.java
 * 用于本地代码调用
 * @author yangxin
 */
public class ClassMethod {
	
	private static void callStaticMethod(String str, int i) {
		System.out.format("ClassMethod::callStaticMethod called!-->str=%s," +
				" i=%d\n", str, i);
	}
	
	private void callInstanceMethod(String str, int i) {
		System.out.format("ClassMethod::callInstanceMethod called!-->str=%s, " +
				"i=%d\n", str, i);
	}
}