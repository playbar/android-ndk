package com.example;

class JavaGreeter {
    public void greet(String[] args) {
        System.out.println("Hello, " + args[0] + " (Java)");
    }
}

class LowLevelGreeter {
    public native void greet(String[] args);
}

class HighLevelGreeter {
    public native void greet(String[] args);
}

public class Hello {
    public static void HelloTest(String[] args) {
        new JavaGreeter().greet(args);
        new LowLevelGreeter().greet(args);
        new HighLevelGreeter().greet(args);
    }
}
