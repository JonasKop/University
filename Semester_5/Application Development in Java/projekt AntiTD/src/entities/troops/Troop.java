package entities.troops;

import java.util.ArrayList;

import entities.EntityTroop;
import player.Player;
import world.World;

/**
 * Troop data class that represents a troop in the game.
 * No logic is programmed here, only data about the troop.
 *
 * @author Martin Hedberg, c17mhg.
 */
public class Troop {

	private int health;
	private String name = "";
	private float speed = 0.2f;
	private String imageName = "";
	private int price = 100;
	private boolean canBeTargeted = true;

	/**
	 * Creates a new troop.
	 *
	 * @param health - The amount of health the troop will have.
	 */
	public Troop(int health) {
		this.health = health;
		troopList.add(this);
	}

	/**
	 * Gets the troops max health.
	 *
	 * @return Troop max health.
	 */
	public int getHealth() {
		return health;
	}

	/**
	 * Gets the troops name.
	 *
	 * @return Troop name.
	 */
	public String getName() {
		return name;
	}

	/**
	 * Sets the troops name and return the troop.
	 *
	 * @param name - Name of the troop.
	 * @return The troop.
	 */
	public Troop setName(String name) {
		this.name = name;
		return this;
	}

	/**
	 * Gets the speed of the troop.
	 *
	 * @return Troop speed.
	 */
	public float getSpeed() {
		return speed;
	}

	/**
	 * Sets the troop speed and return the troop.
	 *
	 * @param speed - The speed of the troop.
	 * @return The troop.
	 */
	public Troop setSpeed(float speed) {
		this.speed = speed;
		return this;
	}

	/**
	 * Gets the name of the image used for the troop.
	 *
	 * @return Name of image.
	 */
	public String getImageName() {
		return imageName;
	}

	/**
	 * Sets price
	 *
	 * @param price - Price
	 * @return Troop
	 */
	public Troop setPrice(int price) {
		this.price = price;
		return this;
	}

	/**
	 * Called when a troop enters a new tile.
	 *
	 * @param entity - Entity that represent the troop.
	 * @param world  - The world in which the entity resides.
	 * @param owner  - The entity's owner, usually the owner.
	 */
	public void onEnterNewTile(EntityTroop entity, World world, Player owner) {

	}

	/**
	 * Gets price
	 *
	 * @return PRICE
	 */
	public int getPrice() {
		return price;
	}

	/**
	 * Can the troop be shot?
	 *
	 * @return True if the troop can be shot.
	 */
	public boolean canBeTargeted() {
		return this.canBeTargeted;
	}

	/**
	 * Sets if the troop can be shot.
	 *
	 * @param b - Can the troop be shot.
	 * @return The troop.
	 */
	public Troop setTargetable(boolean b) {
		this.canBeTargeted = b;
		return this;
	}

	/**
	 * Sets the name of the image file that will represent the troop in game.
	 * Only png files is allowed and no file ending is needed in the name.
	 *
	 * @param imageName - The name of the troop image.
	 * @return The troop.
	 */
	public Troop setImageName(String imageName) {
		this.imageName = imageName;
		return this;
	}

	//Add troops here
	
	public static ArrayList<Troop> troopList = new ArrayList<>();
	public static Troop fastTroop =
			new Troop(3).setName("Canoe").setSpeed(1.0f).setImageName("Canoe")
					.setPrice(150);

	public static Troop tank =
			new Troop(20).setName("Heavy").setSpeed(0.7f).setImageName("Tank")
			.setPrice(300);
	
	public static Troop banker =
			new BankerTroop(5,10).setName("Banker").setSpeed(0.3f).setImageName(
					"Banker").setPrice(450);
	
	public static Troop teleportTroop = new Troop(3).setSpeed(0.5f).setImageName
			("MagicCanoe").setTargetable(false).setName("Ghost").setPrice(700);

}
