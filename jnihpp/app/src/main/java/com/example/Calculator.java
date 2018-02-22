package com.example;

/**
 * Created by houguoli on 2018/2/22.
 */

public class Calculator {
    public Calculator() {
        initialize();
    }

    private long peer;
    protected native void initialize();
    protected native void finalize() throws Throwable;

    public native long add(long a, long b);
    public native long subtract(long a, long b);
}
