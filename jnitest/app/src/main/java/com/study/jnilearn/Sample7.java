package com.study.jnilearn;

/**
 * 综合案例
 * @author yangxin
 *
 */
public class Sample7 {
	
	private Person[] persons = new Person[] {
		new Person("刘备", 50, '男', 65.0f, 1.75),
		new Person("张飞",40, '男', 80.0f, 1.70),
		new Person("关羽", 45, '男', 70.0f, 1.85),
		new Person("赵云", 38, '男', 60.0f, 1.77),
		new Person("黄忠", 56, '男', 65.0f, 1.65),
	};
	
	// 本地方法不受访问权限的限制
	private native static Person queryPerson(String name);	// 根据姓名查询
	private native static void printInfo(Person p);			// 打印详情
	private native int getAvgage();						// 获取平均年龄
	private native String getNames();						// 获取所有姓名
	
	private native static void initIDs();

	public static void main(String[] args) {
		Sample7 s = new Sample7();
		String names = s.getNames();
		System.out.println("人员姓名清单：" + names);
		int avgage = s.getAvgage();
		System.out.printf("平均年龄 %d 岁\n" , avgage);
		Person person = queryPerson("赵云");
		printInfo(person);
	}
	
	static {
		System.loadLibrary("sample7");
		initIDs();
	}
}
