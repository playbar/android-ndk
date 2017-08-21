package com.study.jnilearn;

/**
 * C/C++访问类的实例变量和静态变量
 * @author yangxin
 */
public class AccessField {
	
	private native static void accessInstanceField(ClassField obj);
	
	private native static void accessStaticField();

	public static void main(String[] args) {
		ClassField obj = new ClassField();
		obj.setNum(10);
		obj.setStr("Hello");
		
		// 本地代码访问和修改ClassField为中的静态属性num
		accessStaticField();
		accessInstanceField(obj);
		
		// 输出本地代码修改过后的值
		System.out.println("In Java--->ClassField.num = " + obj.getNum());
		System.out.println("In Java--->ClassField.str = " + obj.getStr());
	}

	static {
		System.loadLibrary("AccessField");
	}
	
}
