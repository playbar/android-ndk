package com.study.jnilearn;

/**
 * ClassField.java
 * 用于本地代码访问和修改该类的属性
 * @author yangxin
 *
 */
public class ClassField {

	private static int num;
	
	private String str;
	
	public int getNum() {
		return num;
	}

	public void setNum(int num) {
		ClassField.num = num;
	}

	public String getStr() {
		return str;
	}

	public void setStr(String str) {
		this.str = str;
	}
}
