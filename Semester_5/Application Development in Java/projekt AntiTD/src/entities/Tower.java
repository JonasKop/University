package entities;

import java.util.ArrayList;

/**
 * @author Martin
 */
public class Tower {

	private int damage = 0;
	private String name;
	private float range;
	private float fireRate;
	private int price;
	private String imageName;

	/**
	 * Creates a new tower.
	 *
	 * @param damage - The damage the towers inflicts on troops.
	 */
	public Tower(int damage) {
		this.damage = damage;
		towerList.add(this);
	}

	/**
	 * Returns the towers damage power.
	 *
	 * @return Damage the towers inflicts.
	 */
	public int getDamage() {
		return damage;
	}

	/**
	 * Gets the towers name.
	 *
	 * @return Towers name.
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
	public Tower setName(String name) {
		this.name = name;
		return this;
	}

	/**
	 * Get the towers range of shooting.
	 *
	 * @return Towers shot range.
	 */
	public float getRange() {
		return range;
	}

	/**
	 * Sets the towers range of shooting.
	 *
	 * @param range - Tower shot range.
	 * @return The tower.
	 */
	public Tower setRange(float range) {
		this.range = Math.max(range, 1f);
		return this;
	}

	/**
	 * Gets the towers rate of fire in shots per second.
	 *
	 * @return The firerate of the tower.
	 */
	public float getFireRate() {
		return fireRate;
	}

	/**
	 * Sets the towers rate of fire in shots per seconds.
	 *
	 * @param fireRate - Rate of fire
	 * @return Tower
	 */
	public Tower setFireRate(float fireRate) {
		this.fireRate = fireRate;
		return this;
	}

	/**
	 * Gets the purchase price of the tower.
	 *
	 * @return The price.
	 */
	public int getPrice() {
		return price;
	}

	/**
	 * Sets the purchase price of the tower.
	 *
	 * @param price - Price to set.
	 * @return The tower.
	 */
	public Tower setPrice(int price) {
		this.price = price;
		return this;
	}

	/**
	 * Gets the name of the image used for the tower.
	 *
	 * @return Name of image.
	 */
	public String getImageName() {
		return imageName;
	}

	/**
	 * Sets the name of the image file that will represent the tower in game.
	 * Only png files is allowed and no file ending is needed in the name.
	 *
	 * @param imageName - The name of the tower image.
	 * @return The tower.
	 */
	public Tower setImageName(String imageName) {
		this.imageName = imageName;
		return this;
	}
	
	//Add towers here

	public static ArrayList<Tower> towerList = new ArrayList<>();
	public static Tower smallTurret = new Tower(1).setName("Turret").setFireRate(1f)
			.setRange(2f).setPrice(100).setImageName("Canon");
	public static Tower largeTurret = new Tower(2).setFireRate(2.f).setRange(5)
			.setPrice(180).setImageName("SuperCanon");
}
