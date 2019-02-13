package entities.effects;

import entities.EntityTroop;
import world.World;

/**
 * Healing effect
 *
 * @author Martin Hedberg, c17mhg
 */
public class EffectHeal extends Effect {

	/**
	 * Creates a heal effect
	 *
	 * @param effectName - Name
	 */
	public EffectHeal(String effectName) {
		super(effectName);
	}

	@Override
	public void onEnterTile(World world, EntityTroop entity) {

	}

	/**
	 * If a unit is on the tile this method runs which heals the entity
	 *
	 * @param deltaTime - Time difference since the last update.
	 * @param world     - The world in which the entity resides.
	 * @param entity    - The entity that is on the tile.
	 */
	@Override
	public void onTile(float deltaTime, World world, EntityTroop entity) {
		entity.heal(deltaTime * 1.f);
	}

	@Override
	public void onExitTile(World world, EntityTroop entity) {

	}

}
