package com.study.jnilearn;

/**
 * 局部引用、全局引用、弱全局引用
 * 1、局部引用和全局引用会阻止GC回收所引用的对象
 * 2、局部引用只在函数内有效，不允许将局部引用声明成静态的，供下次继续使用
 * 3、局部引用不允许保存在全局变量中，在其它函数中引用
 * 4、局引用只在创建它的线程中使用，不允许跨线程调用
 * 
 * 释放局部引用：
 * 	NewLocalRef 创建一个局部引用
 * 	DeleteLocalRef,避免局部引用表溢出
 * 
 * 一个函数内最多可以创建16个局引用，如果想创建更多的局引用，通过以下函数来扩展：
 * EnsureLocalCapacity、NewLocalRef、PushLocalFrame、PopLocalFrame
 * 
 * 定义全局和弱全局引用需要在函数内将变量定义成static的
 * 5、全局引用可以跨函数和跨线程使用
 * 	1> 创建全局引用	NewGlobalRef
 * 	2> 删除全局引用	DeleteGlobalRef
 * 
 * 6、弱全局引用同样可以跨函数和线程使用，但不会阻止GC回收所引用的对象
 * 	1> 创建弱全局引用	NewWeakGlobalRef
 * 	2> 删除弱全局引用	DeleteWeakGlobalRef，如果没有调用，GC会删除所引用的对象占用的内存空间，但引用变量本身在引用表中所占用的内存永远不会删除
 * 
 * 手动释放：
 * DeleteLocalRef，因为局部引用会阻止GC回收所引用的对象
 * @author yangxin
 *
 */
public class Sample4 {

	private native String newString(char[] chars, int len);
	
	public native void testRefresh(int len);
	
	public static void main(String[] args) {
		Sample4 sample4 = new Sample4();
		sample4.testRefresh(10);
		
		char chars[] = {'h','e','l','l','o','c','p','p'}; 
		Sample4 s = new Sample4();
		String str1 = s.newString(chars,4);
		System.out.println(str1);
	}
	
	static {
		System.loadLibrary("Sample4");
	}

}
