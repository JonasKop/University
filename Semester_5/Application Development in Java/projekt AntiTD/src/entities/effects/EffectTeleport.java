package entities.effects;

import entities.EntityTroop;
import primitives.Position;
import world.World;

/**
 * Teleport effect.
 *
 * @author Martin Hedberg, c17mhg
 */
public class EffectTeleport extends Effect {

	/**
	 * Creates a teleport effect.
	 *
	 * @param effectName - Name.
	 */
	public EffectTeleport(String effectName) {
		super(effectName);
	}

	@Override
	public void onEnterTile(World world, EntityTroop entity) {

	}

	/**
	 * If a unit has entered the tile they are teleported 5 tiles forward
	 *
	 * @param deltaTime - Time difference since the last update.
	 * @param world     - The world in which the entity resides.
	 * @param entity    - The entity that is no the tile.
	 */
	@Override
	public void onTile(float deltaTime, World world, EntityTroop entity) {
		Position telePos = entity.getPos();
		for (int i = 0; i < 5; i++) {
			telePos = world.getNextRoad(telePos);
			if (world.getGoalPositions().contains(telePos)) {
				break;
			}
		}
		telePos = telePos.getCenteredPosition();
		entity.setPos(telePos);
	}

	@Override
	public void onExitTile(World world, EntityTroop entity) {

	}

}
