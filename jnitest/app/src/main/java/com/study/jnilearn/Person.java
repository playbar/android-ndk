package com.study.jnilearn;

public class Person {

	public Person() {
		counter++;
		System.out.println("Person Construct call");
	} 
	
	public Person(String name, int age, char sex, float weight, double height) {
		super();
		counter++;
		this.name = name;
		this.age = age;
		this.sex = sex;
		this.weight = weight;
		this.height = height;
	}
	
	private String name;
	private int age;
	private char sex;
	private float weight;
	private double height;
	private static int counter = 0;
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	public char getSex() {
		return sex;
	}
	public void setSex(char sex) {
		this.sex = sex;
	}
	public float getWeight() {
		return weight;
	}
	public void setWeight(float weight) {
		this.weight = weight;
	}
	public double getHeight() {
		return height;
	}
	public void setHeight(double height) {
		this.height = height;
	}

	@Override
	public String toString() {
		return "Person [name=" + name + ", age=" + age + ", sex=" + sex
				+ ", weight=" + weight + ", height=" + height + "]";
	}

}
