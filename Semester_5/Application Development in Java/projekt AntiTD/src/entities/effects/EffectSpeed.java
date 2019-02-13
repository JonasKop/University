package entities.effects;

import entities.EntityTroop;
import world.World;

/**
 * Speed effect
 *
 * @author Martin Hedberg, c17mhg
 */
public class EffectSpeed extends Effect {

	/**
	 * Creates a speed effect.
	 *
	 * @param effectName - Name.
	 */
	public EffectSpeed(String effectName) {
		super(effectName);
	}

	@Override
	public void onEnterTile(World world, EntityTroop entity) {

	}

	/**
	 * If the tile has been entered by a unit its speed is multiplied by 2
	 *
	 * @param deltaTime - Time difference since the last update.
	 * @param world     - The world in which the entity resides.
	 * @param entity    - The entity that is no the tile.
	 */
	@Override
	public void onTile(float deltaTime, World world, EntityTroop entity) {
		entity.setSpeedMultiplier(2.f);
	}

	@Override
	public void onExitTile(World world, EntityTroop entity) {
		entity.setSpeedMultiplier(1.f);
	}

}
