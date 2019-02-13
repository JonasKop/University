package view;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * A view containing a run button, a clear button a textfield taking user
 * input and a textarea where output is written. It can also display a
 * notification to the user using a message dialog.
 */
public class MyView implements View {

    private JFrame frame = new JFrame("MyUnitTester");
    private JButton runButton = new JButton("Run test"),
            clearButton = new JButton("Clear");
    private JTextField textField = new JTextField();
    private JTextArea textArea = new JTextArea();

    /**
     * Initializes the GUI by setting its size and some other options. It
     * then adds the panels to the frame completing the GUI
     */
    public void initialize() {
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setMinimumSize(new Dimension(500, 400));
        frame.setLocationRelativeTo(null);

        textArea.setEditable(false);

        JPanel panel = new JPanel(new BorderLayout());
        panel.add(getNorthPanel(), BorderLayout.NORTH);
        panel.add(new JScrollPane(textArea), BorderLayout.CENTER);
        frame.add(panel);
    }

    /**
     * Creates a JPanel containing a run button, a textfield and a clear
     * button. It then adds an action listener to both buttons which
     * clears the text in the textarea. Then it configs the layout and
     * returns the panel.
     *
     * @return the northern panel containin a run button, a textfield and a
     * clear button
     */
    private JPanel getNorthPanel() {
        clearButton.addActionListener(e -> textArea.setText(null));
        runButton.addActionListener(e -> textArea.setText(null));
        clearButton.setPreferredSize(new Dimension(90, 30));
        runButton.setPreferredSize(new Dimension(90, 30));

        JPanel northPanel = new JPanel();
        northPanel.setLayout(new BorderLayout());
        northPanel.add(runButton, BorderLayout.WEST);
        northPanel.add(textField, BorderLayout.CENTER);
        northPanel.add(clearButton, BorderLayout.EAST);
        return northPanel;
    }

    /**
     * Displays the GUI to the user
     */
    public void display() {
        frame.setVisible(true);
    }

    /**
     * Returns the text in the textfield.
     *
     * @return text in textfield
     */
    @Override
    public String getInput() {
        return textField.getText();
    }

    /**
     * Displays a string in the set textArea.
     *
     * @param str the string that should be displayed
     */
    @Override
    public void displayString(String str) {
        textArea.append(str);
    }

    /**
     * Adds an action listener to the run button
     *
     * @param listener the action listener which should be added
     */
    public void addActionListener(ActionListener listener) {
        runButton.addActionListener(listener);
    }

    /**
     * Notifies the user by showing a dialog presenting the string str
     *
     * @param str the message which should be notified
     */
    @Override
    public void notifyUser(String str) {
        JOptionPane.showMessageDialog(frame, str);
    }
}
