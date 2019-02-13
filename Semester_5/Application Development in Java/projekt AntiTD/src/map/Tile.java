package map;

import primitives.GroundType;
import primitives.Position;
import world.World;

import java.util.ArrayList;

import entities.effects.Effect;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: A Tile in a map. This is made for easy XML translations where
 * a map is specified. It contains its ground-type, its position, if it is
 * a road or not and where it leads, if it is a start and if it is a goal.
 * Used in GameMap.
 */
public class Tile implements LandOn {
	private Position position;
	private GroundType ground;
	private ArrayList<Position> road;
	private Boolean start, goal;
	private ArrayList<Effect> effects;
	private ArrayList<Effect> originalEffects;

	public Tile(float x, float y, GroundType ground, ArrayList<Position> road
			, boolean start, boolean goal, ArrayList<Effect> effects) {
		position = new Position(x, y);
		this.ground = ground;
		this.road = road;
		this.start = start;
		this.goal = goal;
		this.effects = effects;
		this.originalEffects = new ArrayList<>(effects);
	}

	/**
	 * Rotates the tile, making its units go in that direction.
	 *
	 * @param world reference to current world
	 */
	public void onTileClicked(World world) {
		if (this.isRoad() && this.road.size() > 1) {
			rotateNextRoad();
		}
		if (this.isStart()) {
			world.setStartPosition(this);
		}
	}

	/**
	 * Adds an effect to the tile.
	 *
	 * @param e - The effect to be added.
	 * @return Reference to the modified tile.
	 */
	public Tile addEffect(Effect e) {
		this.effects.add(e);
		return this;
	}

	public void resetEffects() {
		this.effects = new ArrayList<>(this.originalEffects);
	}

	/**
	 * Gets the ground type of the tile
	 *
	 * @return a groundtype enum
	 */
	public GroundType getGround() {
		return ground;
	}

	/**
	 * Checks if the tile is a road
	 *
	 * @return true if road, else false
	 */
	public Boolean isRoad() {
		return road != null;
	}

	/**
	 * Returns the next position of the road
	 *
	 * @return the next Position
	 */
	public ArrayList<Position> getRoad() {
		return road;
	}

	//TODO RoadTileClass?
	private int nextRoad = 0;

	public Position getChoosenRoad() {
		if (this.road == null) {
			return null;
		}
		return this.road.get(nextRoad);
	}

	public void rotateNextRoad() {
		this.nextRoad += 1;
		this.nextRoad = this.nextRoad % this.road.size();
	}

	/**
	 * Checks if the tile is a start
	 *
	 * @return true if start, else false
	 */
	public boolean isStart() {
		return start;
	}

	/**
	 * Checks if the tile is a goal
	 *
	 * @return true if goal, else false
	 */
	public Boolean isGoal() {
		return goal;
	}

	/**
	 * Gets the position of the tile
	 *
	 * @return the position of the tile
	 */
	public Position getPosition() {
		return position;
	}

	/**
	 * Generates hashCode for the Tile using its positions hashCode
	 *
	 * @return its positions hashCode
	 */
	@Override
	public int hashCode() {
		return position.hashCode();
	}

	@Override
	public ArrayList<Effect> landOn() {
		return effects;
	}
}
