package com.study.jnilearn;

/**
 * 本地代码线程同步
 * @author yangxin
 *
 */
public class ThreadSync {
	
	private native static void initIDs();

	public static void main(String[] args) {
		Object object;
	}
	
	static {
		System.loadLibrary("threadsync");
		initIDs();
	}

}
