package gui;

import javax.swing.*;

import database.Score;
import entities.troops.Troop;

import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 * @author Marcus Jonsson (c17mjn)
 * @version 1.0
 * Email: c17mjn@cs.umu.se
 * Date: December 2018
 * <p>
 * Description: Main view of program, contains user interface and game view.
 */
public class Window {

	private JFrame frame, newGameWindow, winFrame;
	private JTextArea gameInfo;
	private JPanel troopPanel, levelList;
	private JButton scoreButton = new JButton("Save score");
	private JMenuItem restartAlternative, pauseAlternative, quitAlternative;
	private GameWindow gameWindow = new GameWindow();
	private JTextField userName;
	private ArrayList<JButton> troopButtons = new ArrayList<>();


	/*
	 * Generates the interface and creates a gameWindow object.
	 */
	public Window() {

		//Menu
		JMenuBar menuBar = createMenu();

		//New Game Panel
		levelList = createLevelPanel();

		//Troop Panel
		troopPanel = createTroopPanel();

		//Full window
		frame = new JFrame("Pirates of Nydalasjön");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setJMenuBar(menuBar);
		frame.add(gameWindow, BorderLayout.CENTER);
		frame.add(troopPanel, BorderLayout.SOUTH);
		frame.setMinimumSize(new Dimension(1150, 850));
		frame.setLocationRelativeTo(null);
	}


	/**
	 * Method adding a button to the bottom troop panel.
	 *
	 * @param t        Troop to be added.
	 * @param listener ActionListener for the button press.
	 * @param image    Image to be shown on button together with name.
	 */
	public void addTroopButton(Troop t, ActionListener listener, ImageIcon image) {

		JButton button = new JButton(t.getName());
		button.addActionListener(listener);
		button.setPreferredSize(new Dimension(130, 50));
		button.setIcon(new ImageIcon(image.getImage().getScaledInstance(50, 50, Image.SCALE_DEFAULT)));
		troopPanel.add(button);
		troopButtons.add(button);

		JTextArea troopInfo = new JTextArea();
		troopInfo.append("Cost: " + t.getPrice() + "\n");
		troopInfo.append("HP: " + t.getHealth() + "\n");
		troopInfo.append("Speed: " + t.getSpeed());
		troopInfo.setEditable(false);
		troopInfo.setOpaque(false);
		troopPanel.add(troopInfo);
	}


	/**
	 * Method enabling all troop buttons.
	 */
	public void resetButtons() {
		for (JButton button : troopButtons) {
			button.setEnabled(true);
		}
	}


	/**
	 * Method adding a button to the level selection screen.
	 *
	 * @param name     Level name.
	 * @param listener ActionListener for button press.
	 */
	public void addLevelButton(String name, ActionListener listener) {
		JButton button = new JButton(name);
		button.setAlignmentX(JButton.CENTER_ALIGNMENT);
		button.addActionListener(listener);
		button.addActionListener((e) -> newGameWindow.dispose());
		levelList.add(button);
	}


	/**
	 * method making the frame visible.
	 */
	public void showGUI() {
		frame.setVisible(true);
	}


	/**
	 * Method setting game background.
	 *
	 * @param background BufferedImage containing background image.
	 */
	public void setBackground(BufferedImage background) {
		gameWindow.setBackground(background);
	}


	/**
	 * Method updating the gameWindows active game frame.
	 *
	 * @param gameFrame BufferedImage containing new active game frame.
	 */
	public void updateFrame(BufferedImage gameFrame) {
		gameWindow.updateFrame(gameFrame);
	}


	/**
	 * Method creating a JMenuBar containing a menu and an about button and a help button.
	 *
	 * @return created JMenuBar.
	 */
	private JMenuBar createMenu() {

		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Menu");
		JMenuItem newGameAlternative = new JMenuItem("New game");
		newGameAlternative.addActionListener(actionEvent -> newGame());
		menu.add(newGameAlternative);
		restartAlternative = new JMenuItem("Restart");
		menu.add(restartAlternative);
		pauseAlternative = new JMenuItem("Pause/Resume");
		menu.add(pauseAlternative);
		quitAlternative = new JMenuItem("Quit");
		menu.add(quitAlternative);

		JButton about = new JButton("About");
		about.addActionListener(actionEvent -> aboutPanel());
		JButton help = new JButton("Help");
		help.addActionListener(actionEvent -> helpPanel());

		menuBar.add(menu);
		menuBar.add(about);
		menuBar.add(help);

		return menuBar;
	}


	/**
	 * Method used to set ActionListeners for menu alternatives.
	 *
	 * @param restartListener ActionListener for restart alternative.
	 * @param pauseListener   ActionListener for pause/resume alternative.
	 * @param quitListener    ActionListener for quit alternative.
	 */
	public void setMenuAlternatives(ActionListener restartListener, ActionListener pauseListener, ActionListener quitListener) {
		restartAlternative.addActionListener(restartListener);
		pauseAlternative.addActionListener(pauseListener);
		quitAlternative.addActionListener(quitListener);
	}


	/**
	 * Method used to set ActionListener for "Save score" button in endGame() window.
	 *
	 * @param scoreListener ActionListener for button press.
	 */
	public void setScoreButton(ActionListener scoreListener) {
		scoreButton.addActionListener(scoreListener);
		scoreButton.addActionListener((e) -> winFrame.dispose());
	}


	/**
	 * Method creating and showing window containing general information about the program to the user.
	 */
	private void aboutPanel() {
		String authorString = "Authors:\n"
				+ "Jonas Sjödin\n"
				+ "Marcus Jonsson\n"
				+ "Martin Hedberg\n"
				+ "Petter Skogh\n";

		String dateString = "Created 20-12-2018";

		JTextArea textArea = new JTextArea(authorString + dateString);
		textArea.setEditable(false);
		JFrame aboutFrame = new JFrame();
		aboutFrame.add(textArea);
		aboutFrame.setMinimumSize(new Dimension(200, 150));
		aboutFrame.setLocationRelativeTo(null);
		aboutFrame.setVisible(true);
	}


	/**
	 * Method creating and showing a level selection window to the user containing all available levels.
	 */
	public void newGame() {

		newGameWindow = new JFrame("Select a level to play");

		JScrollPane levelSelectPanel = new JScrollPane(levelList,
				ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
				ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		newGameWindow.add(levelSelectPanel);
		newGameWindow.setMinimumSize(new Dimension(300, 300));
		newGameWindow.setLocationRelativeTo(null);
		newGameWindow.setVisible(true);
	}


	/**
	 * Method creating and showing window containing helpful information to the user.
	 */
	private void helpPanel() {

		JTextArea textArea = new JTextArea();
		BufferedReader instructions;
		try {
			instructions = new BufferedReader(new InputStreamReader(getClass().getResource("Help.txt").openStream()));
			String line;
			while ((line = instructions.readLine()) != null) {
				textArea.append(line + "\n");
			}
		} catch (IOException e) {
			textArea.append("Help file could not be found.");
		}
		textArea.setEditable(false);
		JFrame helpFrame = new JFrame();
		helpFrame.add(new JScrollPane(textArea,
				ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
				ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER));
		helpFrame.setMinimumSize(new Dimension(610, 480));
		helpFrame.setLocationRelativeTo(null);
		helpFrame.setVisible(true);
	}


	/**
	 * Method creating a screen congratulating user for winning and showing current scorelist,
	 * allows the user to enter a name and save it's score.
	 *
	 * @param scoreList   - Current scores for relevant level.
	 * @param playerScore - The scoreeeeee
	 */
	public void endGame(ArrayList<Score> scoreList, int playerScore) {

		//Frame
		winFrame = new JFrame("You won!");
		winFrame.setMinimumSize(new Dimension(400, 500));
		winFrame.setLocationRelativeTo(null);

		//Top panel
		JPanel topPanel = new JPanel();
		topPanel.setLayout(new FlowLayout(FlowLayout.LEFT));

		userName = new JTextField("Insert name here");

		topPanel.add(userName);
		topPanel.add(scoreButton);
		topPanel.add(new JLabel("Your score: " + String.valueOf(playerScore)));
		winFrame.add(topPanel, BorderLayout.NORTH);

		//Center panel
		JPanel middlePanel = new JPanel();
		middlePanel.setLayout(new BorderLayout());

		JTextArea userScoresArea = new JTextArea("Scorelist\n");
		userScoresArea.setFont(new Font("monospaced", Font.PLAIN, 12));
		if (scoreList != null) {
			for (Score score : scoreList) {
				userScoresArea.append(String.format("%-40s: %d\n",
						score.getName(),
						score.getScore()));
			}
		} else {
			userScoresArea.append("Could not connect to database");
		}

		userScoresArea.setEditable(false);

		JScrollPane feedbackWindow = new JScrollPane(userScoresArea,
				ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
				ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);

		middlePanel.add(feedbackWindow, BorderLayout.CENTER);
		winFrame.add(middlePanel, BorderLayout.CENTER);

		winFrame.setVisible(true);
	}


	/**
	 * Method creating a JPanel used to store troop buttons.
	 *
	 * @return Created JPanel.
	 */
	private JPanel createTroopPanel() {

		JPanel troopPanel = new JPanel();
		gameInfo = new JTextArea();
		gameInfo.append(String.format("Credits: %-6s\nEnemy HP: %-6s\nTime: " +
				"%-6s", 0, 0, 0));
		troopPanel.add(gameInfo, FlowLayout.LEFT);
		gameInfo.setEditable(false);
		gameInfo.setOpaque(false);
		troopPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
		return troopPanel;
	}


	/**
	 * Method updating game info in troop panel;
	 *
	 * @param credits Current credit value.
	 * @param enemyHP Current enemy HP.
	 * @param time    Current gametime.
	 */
	public void updateGameInfo(int credits, int enemyHP, String time) {
		gameInfo.setText(String.format("Credits: %-6s\nEnemy HP: %-6s\nTime: " +
				"%-6s", credits, enemyHP, time));
	}

	/**
	 * Method creating a JPanel used to store level buttons.
	 *
	 * @return Created JPanel.
	 */
	private JPanel createLevelPanel() {
		JPanel panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
		return panel;
	}


	/**
	 * Method used to set a MouseListener on gameWindow.
	 *
	 * @param listener MouseListener to be set on gameWindow.
	 */
	public void setMouseListener(MouseListener listener) {
		gameWindow.addMouseListener(listener);
	}


	/**
	 * Method used to get entered name from user in endGame() window.
	 *
	 * @return String containing user's entry.
	 */
	public String getUserName() {
		return userName.getText();
	}


	/**
	 * Method used to get gameWindow's frame width.
	 *
	 * @return Width of gameWindow's frame.
	 */
	public int getFrameWidth() {
		return gameWindow.getWidth();
	}


	/**
	 * Method used to get gameWindow's frame height.
	 *
	 * @return Height of gameWindow's frame.
	 */
	public int getFrameHeight() {
		return gameWindow.getHeight();
	}
}