package map;

import primitives.Position;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: The GameMap. This is data which is read from preferably an XML
 * file. It contains the name of the gamemap, its width, its height, its
 * start-positions, its goal-positions and its the gamemap. At
 * initialization name and size is set. Tiles should be added to
 * make up the X*Y size to make a correct grid.
 */
public class GameMap {
	private String name;
	private int width, height;
	private HashMap<Integer, Tile> gameMap = new HashMap<>();
	private ArrayList<Position> starts = new ArrayList<>();
	private ArrayList<Position> goals = new ArrayList<>();

	GameMap(String name, int width, int height) {
		this.name = name;
		this.width = width;
		this.height = height;
	}

	/**
	 * Adds a tile to the gameMap, use only at initialization.
	 *
	 * @param sq the tile that should be added
	 */
	void addTile(Tile sq) {
		if (gameMap.get(sq.getPosition().hashCode()) == null) {
			gameMap.put(sq.hashCode(), sq);
		}
	}

	/**
	 * Adds a start to the gamemap at following position
	 *
	 * @param p the position of the start
	 */
	void addStart(Position p) {
		starts.add(p.getCenteredPosition());
	}

	/**
	 * Adds a goal to the gamemap at following position
	 *
	 * @param p the position of the goal
	 */
	void addGoal(Position p) {
		goals.add(p);
	}

	/**
	 * Gets a tile at a certain coordinate if it exists at position, else
	 * returns null
	 *
	 * @param x x-coordinate
	 * @param y y-coordinate
	 * @return tile at given coordinate if exists, else null
	 */
	public Tile getTile(float x, float y) {
		return gameMap.get(new Position((float) Math.floor(x), (float) Math.floor(y)).hashCode());
	}

	/**
	 * Gets the goal positions of the gamemap
	 *
	 * @return goal positions of gamemap
	 */
	public ArrayList<Position> getGoals() {
		return goals;
	}

	/**
	 * Gets the start positions of the gamemap
	 *
	 * @return start positions of gamemap
	 */
	public ArrayList<Position> getStarts() {
		return starts;
	}

	/**
	 * Gets the name of the gamemap
	 *
	 * @return name of gamemap
	 */
	public String getName() {
		return name;
	}

	/**
	 * Gets the width of the gamemap
	 *
	 * @return width of gamemap
	 */
	public int getWidth() {
		return width;
	}

	/**
	 * Gets the height of the gamemap
	 *
	 * @return height of gamemap
	 */
	public int getHeight() {
		return height;
	}
}
