package com.study.jnilearn;

/**
 * Native异常处理
 * 注意：
 * 1、Native的异常和Java处理的流程不一样，如果C++或C与Java层交互发生了异常，不会立即终止程序的运行，
 * 而会继续往下执行，所以在C++层调用完Java层的接口，需要检查是否引发了异常，如果发生了异常，需要手动处理
 * 2、必须先检查、处理、清除异常后再做其它JNI 函数调用,否则结果未知
 * @author yangxin
 *
 */
public class Sample5 {
	
	public native void doit() throws IllegalArgumentException;
	public void callback() throws NullPointerException {
		throw new NullPointerException("Sample5.callback");
	}
	
	public native void errorCodeCheck();
	public int floor() {
		return 10 / 2;
	}
	
	public void printExcepiton(Exception e) {
		System.out.println("printExcepiton: " + e);
	}
	
	public static void main(String[] args) {
		Sample5 s = new Sample5();
		try {
			s.doit();
		} catch (IllegalArgumentException e) {
			System.out.println("In Java:\n\t" + e);
		}
		
		//s.errorCodeCheck();
	}

	static {
		System.loadLibrary("sample5");
	}
}
