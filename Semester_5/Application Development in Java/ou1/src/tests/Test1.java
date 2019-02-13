package tests;

import se.umu.cs.unittester.TestClass;

public class Test1 implements TestClass {
    private MyImprovedInt value;

    public Test1() { }

    public void setUp() {
        value = new MyImprovedInt();
    }

    public void tearDown() {
        value = null;
    }

    public boolean testInitialisation() {
        return value.getValue() == 0;
    }

    public boolean testIncrement() {
        value.increment();
        value.increment();
        return value.getValue() == 2;
    }

    public boolean testDecrement() {
        value.decrement();
        value.decrement();
        return value.getValue() == -2;
    }

    public boolean testAdd() {
        value.add(5);
        value.add(87);
        return value.getValue() == (5 + 87);
    }

    public boolean testSubtract() {
        value.subtract(70);
        value.subtract(32);
        return value.getValue() == (-70 - 32);
    }

    public boolean testDivide() {
        value.add(27);
        value.divide(6);
        return value.getValue() == (27 / 6);
    }

    public boolean testMultiply() {
        value.add(2);
        value.multiply(5);
        value.multiply(87);
        return value.getValue() == (2 * 5 * 87);
    }

    public boolean testFailingByException() {
        value = null;
        value.multiply(2);
        return true;
    }

    public boolean testFailing() {
        return false;
    }
}