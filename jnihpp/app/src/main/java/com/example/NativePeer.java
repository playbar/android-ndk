package com.example;



public class NativePeer {
    public static void PeerTest(String[] args) {

        Calculator calculator = new Calculator();
        System.out.println("2 + 2 = " + calculator.add(2, 2));
        System.out.println("8 - 4 = " + calculator.subtract(8, 4));

        // You wouldn't normally use this; it's here to show that the native finalizer does get executed.
        System.runFinalizersOnExit(true);
    }
}
