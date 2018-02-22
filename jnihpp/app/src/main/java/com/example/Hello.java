package com.example;


public class Hello {
    public static void HelloTest(String[] args) {
        new JavaGreeter().greet(args);
        new LowLevelGreeter().greet(args);
        new HighLevelGreeter().greet(args);
    }
}
