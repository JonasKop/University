package model;

import java.lang.reflect.InvocationTargetException;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Exception handler class which takes should be accessed statically since it is
 * never changing. It therefore has a private constructor and CANNOT be
 * instantiated. It only contains one public function called handleException()
 * which takes an exception as an argument and returns a formatted string
 * containing the exception that has been handled and an explanation to that
 * exception.
 */
final class ExceptionHandler {

    private ExceptionHandler() {
    }

    /**
     * Takes an exception and creates an explanation to that exception which it
     * returns
     *
     * @param e the exception that should be explained
     * @return a string explaining the error that was thrown
     */
    static String handleException(Exception e) {
        if (e instanceof ClassNotFoundException) {
            return createErrorMessage(e, "Could not find the test class");
        } else if (e instanceof NoSuchMethodException) {
            return createErrorMessage(e, "A method could not be run");
        } else if (e instanceof InstantiationException) {
            return createErrorMessage(e, "Could not instantiate class");
        } else if (e instanceof IllegalAccessException) {
            return createErrorMessage(e, "Could not access");
        } else if (e instanceof InvocationTargetException) {
            return createErrorMessage(e, "Could not invoke the target method");
        }
        return createErrorMessage(e, "An unexpected exception was thrown");
    }

    /**
     * Creates a formatted error message by using the exceptions name + a set
     * string containing an explanation to the error and a potential error
     * message.
     *
     * @param ex  the exception that should be explained
     * @param str the string that explains the exception
     * @return string containing an explanation to the exception
     */
    private static String createErrorMessage(Exception ex, String str) {
        return ex.getClass().getSimpleName() + ": " + str +
                (ex.getMessage() != null ? " : " + ex.getMessage() : "") + '\n';
    }
}
