package view;

import java.awt.event.ActionListener;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * A View interface which can be implemented for easy gui change with the same
 * backend. It needs to be able to return the input of a field and display a
 * string onto a field. An eventhandler which is using the getInput() method for
 * input and displayString() for output is a good use case of this view. The
 * view should also contain a notification method called notifyUser() which
 * takes a string as an argument which notifies the user of something.
 */
public interface View {

    /**
     * Initializes the view and sets variables which should be set
     * at start.
     */
    void initialize();

    /**
     * Display the GUI window.
     */
    void display();

    /**
     * Displays a string in a field.
     * @param str the string that should be displayed
     */
    void displayString(String str);

    /**
     * Gets the input from an input field
     * @return input from a field
     */
    Object getInput();

    /**
     * Adds an action listener to a object
     * @param listener the action listener which should be added
     */
    void addActionListener(ActionListener listener);

    /**
     * Notifies the user of a change by using the message str.
     * @param str the message which should be notified
     */
    void notifyUser(String str);
}
