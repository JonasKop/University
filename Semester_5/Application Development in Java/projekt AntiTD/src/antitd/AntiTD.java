package antitd;

import java.awt.event.ActionListener;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

import database.Database;
import database.Score;
import entities.troops.Troop;
import graphics.GraphicsGenerator;
import gui.Window;
import map.GameMap;
import map.MapException;
import map.ParseMap;
import player.EnemyAI;
import player.Player;
import primitives.Status;

import static primitives.Status.*;

import world.World;
import antitd.eventHandler.*;

/**
 * @author Marcus Jonsson (c17mjn), Jonas Sj�din (id16jsn)
 * @version 1.0
 * Email: c17mjn@cs.umu.se, id16jsn@cs.umu.se
 * Date: December 2018
 * <p>
 * Description: The controller class which handles the entire game. It has a
 * graphics updater thread, an EDT and a update thread that updates the logic
 * of the game. Resource hungry action listeners are set to run on a separate
 * thread instead of on the EDT.
 */
public class AntiTD {

	private static final int POINTS_NEEDED = 30;
	private World world;
	private Window window;
	private EnemyAI opponent;
	private Player player;
	private GameMap currentMap = null;
	private long startTime = System.currentTimeMillis(), pauseTime, endTime,
			lastTime = System.nanoTime();
	private Status status = QUIT;
	private int playerScore = 0;

	/**
	 * Constructor which initializes the game and creates the game window.
	 *
	 * @param maps the gamemaps that should be played.
	 */
	public AntiTD(ArrayList<GameMap> maps) {
		this.window = new Window();
		ActionListener listener;
		for (Troop t : Troop.troopList) {
			if (t == Troop.teleportTroop) {
				listener = new TeleportActionListener(this);
			} else {
				listener = e -> {
					if (getWorld() != null && !isGamePaused()) {
						getWorld().spawnTroop(t);
					}
				};
			}
			window.addTroopButton(t, listener,
					GraphicsGenerator.loadImageIcon(t.getImageName()));
		}

		window.setScoreButton((e) -> new Thread(() -> {
			try {
				Database.insertHighScore(new Score(window.getUserName(),
						getScore()), currentMap.getName());
			} catch (SQLException | ClassNotFoundException e1) {
				e1.printStackTrace();
			}
		}).start());

		window.setMenuAlternatives(
				(e) -> restartGame(),
				(e) -> pauseResume(),
				(e) -> quitGame());

		for (GameMap m : maps) {
			window.addLevelButton(m.getName(),
					(e) -> new Thread(() -> startNewGame(m)).start());
		}
		this.window.setMouseListener(new TilePressedListener(this));

		this.window.showGUI();

		this.window.newGame();
	}

	/**
	 * Gets the score
	 *
	 * @return the score
	 */
	private int getScore() {
		return (int) ((endTime - startTime) / 10);
	}

	/**
	 * Runs the graphics thread which only quits when a game is quitted.
	 * Handles pause
	 */
	private void runGraphics() {
		while (status != Status.QUIT) {
			try {
				waitIfPaused();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			this.window.updateFrame(GraphicsGenerator.drawFrame(this));
			this.window.updateGameInfo(player.getCredits(), playerScore, getPassedTime());
			try {
				Thread.sleep(14);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * Initializes a game on the map that should be played
	 *
	 * @param level the map that should be played on
	 */
	private void initialize(GameMap level) {
		player = new Player(1000);
		startTime = System.currentTimeMillis();
		lastTime = System.nanoTime();
		this.world = new World(level, player);
		opponent = new EnemyAI(this.world);
	}

	/**
	 * Starts a new game at the given level
	 *
	 * @param level the map
	 */
	public void startNewGame(GameMap level) {

		currentMap = level;
		quitUpdate();
		initialize(level);
		window.resetButtons();

		this.window.setBackground(GraphicsGenerator.createLevelImage(level));

		updateGame();
	}

	/**
	 * Updates the game by updating the world, handles pause and quit. When a
	 * game has finished it set db queries and shows the user the endgame
	 * screen.
	 */
	private void updateGame() {

		status = Status.ACTIVE;

		new Thread(this::runGraphics).start();

		while (this.world != null) {

			if (status == PAUSE) {
				try {
					waitIfPaused();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

			if (status == QUIT) {

				return;
			}

			float deltaTime = (float) ((System.nanoTime() - lastTime) / 1000000000.d);
			lastTime = System.nanoTime();
			if (deltaTime < 0.01f) {
				try {
					Thread.sleep(20 - (int) (deltaTime * 1000));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

			this.world.updateGame(deltaTime);
			this.opponent.tick(deltaTime);

			playerScore = POINTS_NEEDED - player.getScore();
			if (playerScore < 1) {
				break;
			}
		}
		endTime = System.currentTimeMillis();
		this.status = Status.QUIT;
		ArrayList<Score> scores = null;
		try {
			scores = Database.getHighScores(currentMap.getName());
		} catch (SQLException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		this.window.endGame(scores, getScore());
	}

	/**
	 * Waits if the game is paused
	 *
	 * @throws InterruptedException
	 */
	private synchronized void waitIfPaused() throws InterruptedException {
		if (status == PAUSE) {
			wait();
		}
	}

	/**
	 * Notifies update thread that it should start running if it is waiting
	 */
	private synchronized void notifyIfActive() {
		if (status == ACTIVE) {
			notifyAll();
			lastTime = System.nanoTime(); //Not perfect but ok
		}
	}

	/**
	 * Gets the passed time from start to finish of a game.
	 *
	 * @return passed time from start to finish
	 */
	private String getPassedTime() {
		long endTime = System.currentTimeMillis() - startTime;
		long min = TimeUnit.MILLISECONDS.toMinutes(endTime);
		long sec = TimeUnit.MILLISECONDS.toSeconds(endTime) - TimeUnit.MINUTES
				.toSeconds(TimeUnit.MILLISECONDS.toMinutes(endTime));
		return min == 0 ? String.valueOf(sec) : min + ":" + sec;
	}

	/**
	 * Checks if the game is paused and returns a boolean telling if it is
	 *
	 * @return if game is paused
	 */
	public boolean isGamePaused() {
		return status == Status.PAUSE;
	}

	/**
	 * Pauses the game if it is active and resumes it if it is paused by
	 * notifying the updateGame thread.
	 */
	private void pauseResume() {

		if (status == PAUSE) {
			startTime += System.currentTimeMillis() - pauseTime;
			status = ACTIVE;
			notifyIfActive();
		} else if (status == ACTIVE) {
			status = PAUSE;
			pauseTime = System.currentTimeMillis();
		}
	}

	/**
	 * Resets all tiles effect to their values at load.
	 */
	private void clearTileEffects() {
		for (int i = 0; i < currentMap.getWidth(); i++) {
			for (int j = 0; j < currentMap.getHeight(); j++) {
				currentMap.getTile(i, j).resetEffects();
			}
		}
	}

	/**
	 * Restarts the game by quitting the ongoing update and starting the new
	 * game on a new thread.
	 */
	private void restartGame() {
		if (currentMap != null) {
			quitUpdate();
			new Thread(() -> startNewGame(currentMap)).start();
		}
	}

	/**
	 * Quits the game
	 */
	private void quitGame() {
		System.exit(0);
	}

	/**
	 * Stops updating the updateGame method. Run this by other thread which
	 * notifies the updateThread that it should finish.
	 */
	private synchronized void quitUpdate() {
		if (status == PAUSE) {
			status = ACTIVE;
			notify();
		}
		clearTileEffects();
		status = QUIT;
	}

	public World getWorld() {
		return this.world;
	}

	/**
	 * Gets the current window
	 *
	 * @return current window
	 */
	public Window getWindow() {
		return this.window;
	}

	public static void main(String[] args) {
		String levels = args.length != 0 ? args[0] : "levels.xml";

		try {
			new AntiTD(ParseMap.parse(levels));
		} catch (MapException e) {
			e.printStackTrace();
		}
	}
}
