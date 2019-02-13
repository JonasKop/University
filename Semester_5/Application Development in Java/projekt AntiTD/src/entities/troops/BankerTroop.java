package entities.troops;

import entities.EntityTroop;
import player.Player;
import world.World;

/**
 * Banker troop that adds credits when it moves.
 *
 * @author Martin Hedberg, c17mhg
 */
public class BankerTroop extends Troop {

	private int credidsPerTile;

	/**
	 * Creates a new banker troop
	 *
	 * @param health         - Troop max health.
	 * @param creditsPerTile - How many credits the banker will add per tile moved.
	 */
	public BankerTroop(int health, int creditsPerTile) {
		super(health);
		this.credidsPerTile = creditsPerTile;
	}

	/**
	 * When tile has been entered more credits are added to the player
	 *
	 * @param entity - Entity that represent the troop.
	 * @param world  - The world in which the entity resides.
	 * @param owner  - The entity's owner, usually the owner.
	 */
	@Override
	public void onEnterNewTile(EntityTroop entity, World world, Player owner) {
		super.onEnterNewTile(entity, world, owner);
		owner.addCredits(credidsPerTile);
	}

}
