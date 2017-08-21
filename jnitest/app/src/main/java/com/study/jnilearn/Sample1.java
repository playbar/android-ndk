package com.study.jnilearn;

/**
 * Java调用C接口，测试基本类型、字符串、数组类型
 * @author yangxin
 */
public class Sample1 {

	// 声明本地方法
	public native int intMethod(int num);
	public native boolean booleanMethod(boolean b);
	public native float floatMethod(float f);
	public native String stringMethod(String name);
	public native int intArrayMethod(int[] intArray);	// 统计数组中每个元素的总和
	public native String stringArrayMethod(String[] strArray,String separator);	// 将数组中每个字符串用separator分隔后返回
	public native int[][] init2array(int size);	// 创建一个2维数组
	
	public static void main(String[] args) {
		Sample1 sample = new Sample1();
		System.out.printf("intMethod: %d\n", sample.intMethod(10));
		System.out.printf("booleanMethod: %b\n", sample.booleanMethod(true));
		System.out.printf("floatMethod:%.2f\n", sample.floatMethod(10.5f));
		System.out.printf("stringMethod:%s\n", sample.stringMethod("杨信"));
		System.out.printf("intArrayMethod:%d\n", sample.intArrayMethod(new int[] {1,2,3,4,5,6,7,8,9,10,11,12}));
		System.out.printf("stringArrayMethod:%s\n", sample.stringArrayMethod(new String[] {"刘备","guanyu","张飞",
				"赵云","黄忠","曹操","诸葛亮","周瑜","孙权","司马懿"}, ","));
		int arr[][] = sample.init2array(3);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				System.out.printf(" %d",arr[i][j]);
			}
			System.out.println();
		}
	}
	
	static {
		System.loadLibrary("sample1");	// 加载动态库
	}
}
