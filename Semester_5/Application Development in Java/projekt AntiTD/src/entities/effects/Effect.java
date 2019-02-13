package entities.effects;

import entities.EntityTroop;
import world.World;

/**
 * Abstract class for effects, effects are added to tiles and will affect the 
 * the EntityTroop that is currently walking on that tile.
 * 
 * @author Martin Hedberg, c17mhg
 *
 */
public abstract class Effect {
	
	private String name;
	
	/**
	 * Creates a new effect.
	 * @param effectName - Name of the effect.
	 */
	public Effect(String effectName) {
		this.name = effectName;
	}

	/**
	 * Called once an entity enters a new tile.
	 * @param world - The world in which the entity resides.
	 * @param entity - The entity that steped on the tile.
	 */
	public abstract void onEnterTile(World world, EntityTroop entity);
	
	/**
	 * Called once an entity leaves the tile.
	 * @param world - The world in which the entity resides.
	 * @param entity - The entity that left the tile.
	 */
	public abstract void onExitTile(World world, EntityTroop entity);
	
	/**
	 * Called once every game tick when an entity is on the tile with the effect.
	 * @param deltaTime - Time difference since the last update.
	 * @param world - The world in which the entity resides.
	 * @param entity - The entity that is no the tile.
	 */
	public abstract void onTile(float deltaTime, World world, EntityTroop entity);
	
	/**
	 * Gets the effects name.
	 * @return Effect name.
	 */
	public String getName() {
		return name;
	}

	public static final Effect health = new EffectHeal("Heal");
	public static final Effect speed = new EffectSpeed("Boost");
	public static final Effect teleport = new EffectTeleport("effect_teleport");
}
