package model;

import other.Callback;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * TestModel which has a public run function which runs a test using the
 * input string as input to the test and runs test results through the
 * callback method, preferably for messaging the user about the test results.
 * It can also return an object, preferably a string if a big error occurs
 * or something that the test wants to send when the test quits on error or
 * finishes.
 */
public interface TestModel {

    /**
     * Method which runs a test using the input string input and runs
     * test status or result through the callback function. On finish or exit
     * on error it can return a object, preferably a string which can be
     * used to notify the user of it.
     *
     * @param callback the callback method
     * @param input    the input string to the test
     * @return an object, preferably a notification to the user
     */
    Object run(Callback<String> callback, String input);
}
