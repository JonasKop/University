package tests;

import se.umu.cs.unittester.TestClass;

import java.util.ArrayList;

public class Test2 implements TestClass {
    ArrayList<String> arr = new ArrayList<>();

    public Test2() { }

    public void setUp() {
        arr = new ArrayList<>();
    }

    public void tearDown() {
        arr = null;
    }

    public boolean testIfEmpty() {
        return arr.size() == 0;
    }

    public boolean testAddLast() {
        arr.add("Tjosan");
        arr.add("Berit");
        return arr.get(0).equals("Tjosan")  && arr.get(1).equals("Berit");
    }

    public boolean testRemoveString() {
        arr.add("Tjosan");
        arr.add("Berit");
        arr.remove(0);
        return arr.get(0).equals("Berit");
    }

    public boolean testFindString() {
        arr.add("Tjosan");
        arr.add("Berit");
        arr.sort(String::compareToIgnoreCase);
        return arr.get(0).equals("Berit");
    }
}