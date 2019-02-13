package se.umu.cs.unittester;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * ****************************************************************************
 * Interface for a test class which contains a setUp and a tearDown method. The
 * setUp method should be run before test execution and tearDown after test
 * execution.
 */
public interface TestClass {

    /**
     * Sets up a test class making it ready for testing.
     */
    void setUp();

    /**
     * Tears down a test class, removing it correctly and completely.
     */
    void tearDown();
}