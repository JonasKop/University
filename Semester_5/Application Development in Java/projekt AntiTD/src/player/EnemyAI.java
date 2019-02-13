package player;

import java.util.Random;

import entities.Tower;
import world.World;

/**
 * EnemyAI represents a stupid AI that buys towers.
 *
 * @author Martin Hedberg, c17mhg
 */
public class EnemyAI {

	private float credits;
	private final float CREDIT_RATE = 10;
	private final int MAX_TOWERS = 50;
	private Tower nextTower;
	private Random rand = new Random();
	private World world;
	private int towersPlaced = 0;

	/**
	 * Initiates the enemy AI by setting which should be the next tower and
	 * setting a start value of credits.
	 *
	 * @param world the world controller
	 */
	public EnemyAI(World world) {
		this.world = world;

		nextTower = Tower.smallTurret;
		this.credits = nextTower.getPrice();
	}

	/**
	 * Decides which type of tower the next one should be
	 */
	private void decideTower() {
		int towerSize = Tower.towerList.size();
		this.nextTower = Tower.towerList.get(rand.nextInt(towerSize));
	}

	/**
	 * Gets a tower if it is possible to purchase one with the current amount
	 * of credits. If it can it returns it, else null
	 *
	 * @return a tower if it can be bought else null
	 */
	private Tower getTower() {
		if (this.credits >= nextTower.getPrice()) {
			this.credits -= nextTower.getPrice();
			Tower temp = nextTower;
			decideTower();
			return temp;
		}
		return null;
	}

	/**
	 * A tick in a round of the game by the enemy ai. It tries to spawn a
	 * tower and adds credits to its credit value depending on deltaTime.
	 *
	 * @param deltaTime - You should know this by now.
	 */
	public void tick(float deltaTime) {
		if (towersPlaced < MAX_TOWERS) {
			Tower t = getTower();
			if (t != null) {
				world.spawnTower(t);
				towersPlaced++;
			}
		}

		this.incrementCredits(deltaTime);
	}

	/**
	 * Increments the credits depending of the update interval deltatime
	 *
	 * @param deltaTime the update time
	 */
	private void incrementCredits(float deltaTime) {
		this.credits += deltaTime * this.CREDIT_RATE;
	}
}
