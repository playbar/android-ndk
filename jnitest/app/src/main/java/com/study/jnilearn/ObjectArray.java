package com.study.jnilearn;

public class ObjectArray {

	private native int[][] initInt2DArray(int size);
	
	public static void main(String[] args) {
		ObjectArray obj = new ObjectArray();
		int[][] arr = obj.initInt2DArray(3);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				System.out.format("arr[%d][%d] = %d\n", i, j, arr[i][j]);
			}
		}
	}

	static {
		System.loadLibrary("ObjectArray");
	}
}
