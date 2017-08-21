package com.study.jnilearn;

// 访问基本类型数组
public class IntArray {

	// 在本地代码中求数组中所有元素的和
	private native int sumArray(int[] arr);
	
	public static void main(String[] args) {
		IntArray p = new IntArray();
		int[] arr = new int[10];
		for (int i = 0; i < arr.length; i++) {
			arr[i] = i;
		}
		int sum = p.sumArray(arr);
		System.out.println("sum = " + sum);
	}

	static {
		System.loadLibrary("IntArray");
	}
}
