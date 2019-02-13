package tests;

public class MyImprovedInt {
    private int val;

    public MyImprovedInt() {
        val = 0;
    }

    public void increment() {
        val++;
    }

    public void decrement() {
        val--;
    }

    public void add(int i) {
        val += i;
    }

    public void subtract(int i) {
        val -= i;
    }

    public void divide(int i) {
        val /= i;
    }

    public void multiply(int i) {
        val *= i;
    }

    public int getValue() {
        return val;
    }
}