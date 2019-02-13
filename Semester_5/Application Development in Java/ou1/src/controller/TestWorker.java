package controller;

import model.TestModel;
import view.View;

import javax.swing.*;
import java.util.List;
import java.util.concurrent.ExecutionException;

/**
 * Author: Jonas Sjödin
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Swingworker class which runs a testModel with a callback function. It then
 * runs the views display method on every callbacked string. After the models
 * run method has finished it can display the return value of the run
 * function as a string in a notification field.
 */
public class TestWorker extends SwingWorker<String, String> {

    private View view;
    private TestModel testModel;

    /**
     * Creates a TestWorker and sets a reference to the view and the testModel
     * which should be run
     *
     * @param view      the current view where the output of the testModel
     *                  should be displayed
     * @param testModel the testModel which should contain a run method which
     *                  should be ran
     */
    public TestWorker(View view, TestModel testModel) {
        this.view = view;
        this.testModel = testModel;
    }

    /**
     * Runs the set testModel and returns a potential error string that the run
     * method has created that is retrieved from done()
     *
     * @return error string which should be retrieved from done()
     */
    @Override
    protected String doInBackground() {
        return (String) testModel.run(this::publish, (String) view.getInput());
    }

    /**
     * Runs after doInBackground has completed. If it returned null it wont do
     * anything, else it notifies the user on the message.
     */
    @Override
    protected void done() {
        try {
            String s = get();
            if (s != null) {
                view.notifyUser(s);
            }

        } catch (InterruptedException | ExecutionException e) {
            view.notifyUser("An error occurred in the controller: " +
                    e.getMessage());
        }
    }

    /**
     * Displays the given string that were published to the view.
     *
     * @param list list containing strings that should be displayed
     */
    @Override
    protected void process(List<String> list) {
        for (String str : list) {
            view.displayString(str);
        }
    }
}