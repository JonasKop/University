package model;

import other.Callback;
import se.umu.cs.unittester.TestClass;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Test runner class which has one public method called run. It runs the test
 * and writes to the callback method the status of the tests. If a testclass
 * could not be ran it returns the explanation as to why it could not be ran.
 * otherwise it returns null.
 */
public class TestRunner implements TestModel {

    /**
     * Run method which first gets the input string value as a test class.
     * Then it checks if it implements se.umu.cs.unittester.TestClass. If it
     * doesn't it returns with an explanation to the error. Then it gets all
     * the methods of the test class and retrieves the setUp() and tearDown()
     * methods if they are there. hen it runs a test which should write the
     * test result to the callback function.
     *
     * @param callback the callback method
     * @param input    the name of the test class which should be tested
     * @return null if success, else an explanation to why it could not test
     * the class correctly.
     */
    @Override
    public Object run(Callback<String> callback, String input) {
        try {
            Class<?> cl = Class.forName(input);
            if (!TestClass.class.isAssignableFrom(cl)) {
                return "Class does not implement se.umu.cs.unittester." +
                        "TestClass";
            }

            Method[] methods = cl.getMethods();

            Method setUp = null, tearDown = null;
            for (Method m : methods) {
                if (m.toString().contains(".setUp()")) {
                    setUp = m;
                } else if (m.toString().contains(".tearDown()")) {
                    tearDown = m;
                }
            }
            runTest(cl.getConstructor().newInstance(), methods, setUp, tearDown,
                    callback);
        } catch (Exception ex) {
            return ExceptionHandler.handleException(ex);
        }
        return null;
    }

    /**
     * Runs a testclass. First it runs a setUp method if it exists then a
     * testMethod and the a tearDown method if it exists. If an exception is
     * thrown or a false return value is returned by a test method it
     * callbacks a string containing which test method were run and why it
     * failed. If a test method is successful it callbacks that that test
     * were successful. After all test methods has been ran it callbacks a
     * result containing if all tests were correct or if they failed. If the
     * test class failed it shows how many tests that were successful,
     * potentially how many tests that threw an error and potentially how
     * many tests who returned a false return value indicating a test failure.
     *
     * @param testClass the class that should be tested.
     * @param methods   the methods of that class.
     * @param setUp     the setUp() method
     * @param tearDown  the tearDown() method
     * @param callback  the callBack() method where the test result is
     *                  written to the callback method
     * @throws IllegalAccessException    if it were unable to access the method
     * @throws InvocationTargetException if it could not invoke a method on
     *                                   the class.
     */
    private void runTest(Object testClass, Method methods[], Method setUp,
                         Method tearDown, Callback<String> callback)
            throws IllegalAccessException, InvocationTargetException {
        int nrOfTests = 0, successfulTests = 0, failedTests = 0,
                failedExceptionTests = 0;
        for (Method m : methods) {
            if (setUp != null) {
                setUp.invoke(testClass);
            }
            StringBuilder sb = new StringBuilder(m.getName() + ": ");

            if (m.toString().contains(testClass.getClass().getSimpleName() +
                    ".test")) {
                nrOfTests++;
                try {
                    if ((Boolean) m.invoke(testClass)) {
                        sb.append("SUCCESS");
                        successfulTests++;
                    } else {
                        sb.append("FAILED");
                        failedTests++;
                    }
                } catch (Exception ex) {
                    sb.append("FAILED\n - Generated a ")
                            .append(ex.getCause().getClass().getName());
                    failedExceptionTests++;
                }
                callback.callBack(sb.toString() + "\n");
            }
            if (tearDown != null) {
                tearDown.invoke(testClass);
            }
        }
        callback.callBack(createResult(successfulTests, nrOfTests, failedTests,
                failedExceptionTests));
    }

    /**
     * Creates a result string and returns the result. It contains how many
     * tests that were successful, how many failed and why they failed.
     *
     * @param successfulTests      nr of successful tests
     * @param nrOfTests            nr of total tests
     * @param failedTests          nr of tests that failed by return value
     * @param failedExceptionTests nr of tests that failed by exception
     * @return string containing test result
     */
    private String createResult(int successfulTests, int nrOfTests,
                                int failedTests, int failedExceptionTests) {
        StringBuilder sb = new StringBuilder("\n-------TEST RESULTS-------\n");
        if (successfulTests == nrOfTests) {
            return sb.append("All ").append(nrOfTests)
                    .append(" tests were successful!\n").toString();
        }

        sb.append(successfulTests).append(" out of ").append(nrOfTests).append(
                " tests were ").append("successful\n");
        if (failedTests != 0) {
            sb.append(failedTests).append(" out of ").append(nrOfTests).append(
                    " tests failed with a FALSE return getValue\n");
        }
        if (failedExceptionTests != 0) {
            sb.append(failedExceptionTests).append(" out of ").append(nrOfTests)
                    .append(" tests failed with an EXCEPTION\n");
        }
        return sb.toString();
    }
}
