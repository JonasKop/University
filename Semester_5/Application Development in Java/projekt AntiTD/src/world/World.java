package world;

import primitives.*;
import entities.*;
import entities.effects.Effect;
import entities.troops.Troop;
import map.*;
import player.Player;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

/**
 * @author Petter Skogh (id16psh), Martin Hedberg (c17mhg)
 * @version 1.0
 * Email: id16psh@cs.umu.se, c17mhg@cs.umu.se
 * Date: December 2018
 * <p>
 * Description: The game state for the game. Contains all information of the
 * map and what troops and towers that exist in the world.
 */
public class World {

	private GameMap levelMap;
	private List<EntityTroop> troops =
			Collections.synchronizedList(new ArrayList<>());
	private List<EntityTower> towers =
			Collections.synchronizedList(new ArrayList<>());
	private Position startPosition;
	private ArrayList<Position> startPositions;
	private ArrayList<Position> goalPositions;
	private Random worldRand = new Random();
	private ArrayList<Tile> road = new ArrayList<>();
	private Player player;
	private int nrOfBankers = 0;

	/**
	 * Constructor. Sets variables from the given map and player.
	 *
	 * @param map    - The map that is played
	 * @param player - The player
	 */
	public World(GameMap map, Player player) {
		levelMap = map;
		this.player = player;
		startPositions = levelMap.getStarts();
		startPosition = startPositions.get(0);
		goalPositions = levelMap.getGoals();
		getRoadPositionsFromMap();
	}

	/**
	 * Goes through levelMap and saves all road tiles to road array list.
	 */
	private void getRoadPositionsFromMap() {

		for (int i = 0; i < levelMap.getWidth(); i++) {
			for (int j = 0; j < levelMap.getHeight(); j++) {
				Tile tile = levelMap.getTile(i, j);
				if (tile.isRoad()) {
					this.road.add(tile);
				}
			}
		}
	}

	/**
	 * If the player has enough money to buy the troop an EntityTroop is created
	 * from the given Troop by setting it to current start position and adds it
	 * to troops array list.
	 *
	 * @param newTroop - The troop to be spawned
	 */
	public void spawnTroop(Troop newTroop) {

		if (player.removeCredits(newTroop.getPrice())) {
			EntityTroop troop = new EntityTroop(newTroop, startPosition,
					this.player);
			if (newTroop.getName().equals("Banker")) {
				nrOfBankers++;
			}
			troops.add(troop);
		}
	}

	/**
	 * Creates new position and creates EntityTower from given tower with the
	 * new position, then adds tower to towers array list.
	 *
	 * @param newTower - The tower to be spawned
	 */
	public void spawnTower(Tower newTower) {

		Position newPosition = findNewTowerPosition(newTower);
		EntityTower tower = new EntityTower(newTower, newPosition);
		towers.add(tower);
	}

	/**
	 * Finds a new position for a tower that isn't on a road, has at least one
	 * tile in tower's range and isn't close to another tower.
	 *
	 * @param tower - the tower for which the new position is to be found
	 * @return The new position
	 */
	private Position findNewTowerPosition(Tower tower) {

		Position position = null;

		while (position == null) {
			Position tileToStrike = road.get(worldRand.nextInt(road.size())).
					getPosition().getCenteredPosition();
			Position randomPosAround = getPosInRadius(tileToStrike,
					tower.getRange() * 0.6f, 0.7f);
			Tile foundTile = levelMap.getTile(randomPosAround.getX(),
					randomPosAround.getY());
			if (foundTile != null && !foundTile.isRoad()) {
				if (!isCloseToTower(randomPosAround, 0.5f)) {
					position = randomPosAround;
				}
			}
		}

		return position;
	}

	/**
	 * Gets random point in radius of center position between min and max radius
	 *
	 * @param center    - Position of center of entity
	 * @param maxRadius - Upper limit of radius
	 * @param minRadius - Lower limit of radius
	 * @return The position inside the radius
	 */
	private Position getPosInRadius(Position center, float maxRadius,
									float minRadius) {
		Position angle = new Position(worldRand.nextFloat() * 2 - 1,
				worldRand.nextFloat() * 2 - 1).normalize();
		float dist = minRadius + worldRand.nextFloat() * (maxRadius - minRadius);

		return Position.add(center, Position.multiply(angle, dist));
	}

	/**
	 * Checks if position is within the given distance
	 *
	 * @param pos  - The position to check
	 * @param dist - The distance to check if the position is within
	 * @return true if position is close to tower, else false
	 */
	private boolean isCloseToTower(Position pos, float dist) {

		for (int i = 0; i < towers.size(); i++) {
			if (pos.getDistanceTo(towers.get(i).getPos()) <= dist) {
				return true;
			}
		}

		return false;
	}

	/**
	 * Returns the worlds current level.
	 *
	 * @return The level played
	 */
	public GameMap getCurrentLevel() {
		return levelMap;
	}

	/**
	 * Gives the next position/positions in road that tile on given position
	 * has.
	 *
	 * @param currentPos - The position from which next positions are taken
	 * @return An array list with next positions
	 */
	public ArrayList<Position> getNextTargets(Position currentPos) {
		Tile currentTile = this.levelMap.getTile(currentPos.getX(),
				currentPos.getY());
		return currentTile.getRoad();
	}

	/**
	 * Gets next road position from tile on given position
	 *
	 * @param pos - Position of tile
	 * @return Position of next tile
	 */
	public Position getNextRoad(Position pos) {
		Tile t = this.levelMap.getTile(pos.getX(), pos.getY());
		if (t == null) {
			return null;
		}
		return t.getChoosenRoad();
	}

	/**
	 * Updates tick for all Troops and Towers in world
	 *
	 * @param deltaTime - The between updates.
	 */
	public void updateGame(float deltaTime) {

		for (int i = 0; i < troops.size(); i++) {
			if (troops.get(i).pendingKill()) {

				if (troops.get(i).getTroop().getName().equals("Banker")) {
					nrOfBankers--;
				}
				troops.remove(i--);
			} else {
				troops.get(i).tick(deltaTime, this);
			}
		}

		for (EntityTower tower : towers) {
			tower.tick(deltaTime, this);
		}
		player.addCredits(deltaTime * (2 * 20 + nrOfBankers * 10));
	}

	/**
	 * Checks if tile is tagged as start, if it is then sets startPosition as
	 * tile's position.
	 *
	 * @param tile - The tile which position is to be set as start
	 */
	public void setStartPosition(Tile tile) {
		if (tile.isStart()) {

			startPosition = tile.getPosition().getCenteredPosition();
		}
	}

	/**
	 * Adds an effect to the tile on the given position.
	 *
	 * @param pos    - Position of tile to get effect.
	 * @param effect - Effect to be added, use static objects.
	 */
	public void setEffect(Position pos, Effect effect) {
		this.levelMap.getTile(pos.getX(), pos.getY()).addEffect(effect);
	}

	/**
	 * @return road (positions for road)
	 */
	public ArrayList<Tile> getRoad() {
		return road;
	}

	/**
	 * @return width of levelMap
	 */
	public int getMapWidth() {
		return levelMap.getWidth();
	}

	/**
	 * @return height of levelMap
	 */
	public int getMapHeight() {
		return levelMap.getHeight();
	}

	/**
	 * @return Current start position
	 */
	public Position getStartPosition() {
		return startPosition;
	}

	/**
	 * @return startPositions
	 */
	public ArrayList<Position> getStartPositions() {
		return startPositions;
	}

	/**
	 * @return goalPositions
	 */
	public ArrayList<Position> getGoalPositions() {
		return goalPositions;
	}

	/**
	 * @return towers
	 */
	public List<EntityTower> getTowers() {
		return towers;
	}

	/**
	 * @return troops
	 */
	public List<EntityTroop> getTroops() {
		return troops;
	}

	/**
	 * Gets effect of tile on given position
	 *
	 * @param pos - Position
	 * @return The effect of the tile
	 */
	public ArrayList<Effect> getEffectsOnTile(Position pos) {
		return this.levelMap.getTile(pos.getX(), pos.getY()).landOn();
	}
}
