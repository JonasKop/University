package controller;

import model.TestModel;
import model.TestRunner;
import view.View;
import view.MyView;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Controller class which takes a model and a view. It creates the view and
 * sets an action listener to an object in the view and then displays the GUI
 * to the user
 */
public class Controller {

    /**
     * Controller which takes a testModel and a view as an argument. First it
     * initializes the view, then it adds an action listener to the view
     * using the view and the testModel. Then it displays the view and the GUI
     * window can is shown.
     *
     * @param testModel the testModel which should be used by the TestWorker
     * @param view      the view that should be shown and used by the TestWorker
     */
    public Controller(TestModel testModel, View view) {
        try {
            SwingUtilities.invokeAndWait(() -> {
                view.initialize();
                view.addActionListener(e -> new TestWorker(view, testModel)
                        .execute());
                view.display();
            });
        } catch (InterruptedException | InvocationTargetException e) {
            throw new RuntimeException("Initialization error: " +
                    e.getMessage());
        }
    }

    /**
     * Main class which just starts the controller.
     *
     * @param args not used
     */
    public static void main(String[] args) {
        new Controller(new TestRunner(), new MyView());
    }
}
