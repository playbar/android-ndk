package com.study.jnilearn;

/**
 * 测试本地代码访问Java对象的属性和方法
 * @author yangxin
 *
 */
public class Sample2 {
	
	public Sample2() {
		System.out.println("call Sample2:construct method");
	}
	
	private Person[] persons = new Person[] {
			new Person("刘备",30,'男',65.0f, 1.78f),
			new Person("张飞",29,'男',90.2f, 1.75f),
			new Person("关羽",30,'男',80.2f, 1.80f),
			new Person("孙权",25,'男',70.2f, 1.85f),
			new Person("赵云",28,'男',75f, 1.75f),
	};
	private String name = "yangxin";
	private static int num = 10;
	
	public native void accessStaticField();
	public native static void accessStaticMethod();
	public native static void accessPrimaryInstanceMethod();	// 访问返回基础类型的实例方法
	
	private static void nativeMethod(int num) {
		Sample2.num = num;
		System.out.format("nativeMethod call, args num:%d\n", num);
	}
	
	private int intMethod(int num) {
		System.out.println("native call intMethod");
		return num * num;
	}
	
	private float floatMethod(float f) {
		System.out.println("native call floatMethod");
		return f + f;
	}
	
	private double doubleMethod(double d) {
		System.out.println("native call doubleMethod");
		return d + d;
	}
	
	private boolean booleanMethod(boolean b) {
		System.out.println("native call booleanMethod");
		return !b;
	}
	
	private char charMethod(char c) {
		System.out.println("native call charMethod:"+c);
		return 'a';
	}
	
	private boolean queryPerson(Person person) {
		System.out.println("native call queryPerson:" + person);
		for (Person p : persons) {
			if (p.getName().equals(person.getName())) {
				return true;
			}
		}
		return false;
	}
	
	public static void main(String[] args) {
		Sample2 sample2 = new Sample2();
		Sample2.accessStaticMethod();
		System.out.format("In Java:\nSample2.name=%s\n", sample2.name);
		sample2.accessStaticField();
		System.out.format("In Java:\nSample2.num=%d\n", num);
		
		accessPrimaryInstanceMethod();
	}
	
	static {
		System.loadLibrary("sample2");
	}

}
