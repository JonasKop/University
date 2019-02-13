package entities;

import primitives.Position;
import world.World;

/**
 * The entity class is an abstract class that represents a
 * entity in the world.
 *
 * @author Martin Hedberg, c17mhg
 */
public abstract class Entity {

	private Position pos;
	private double rotation;

	/**
	 * Creates a new entity at the given position.
	 *
	 * @param pos - Entity's initial position.
	 */
	public Entity(Position pos) {
		this.pos = pos;
	}

	/**
	 * Gets the entity's position.
	 *
	 * @return Entity position.
	 */
	public Position getPos() {
		return pos;
	}

	/**
	 * Sets the position of the entity.
	 *
	 * @param pos - New position.
	 */
	public void setPos(Position pos) {
		this.pos = pos;
	}

	/**
	 * Gets the entity's rotation.
	 *
	 * @return Entity rotation.
	 */
	public double getRotation() {
		return rotation;
	}

	/**
	 * Sets the entity's rotation
	 *
	 * @param rotation New rotation.
	 */
	public void setRotation(double rotation) {
		this.rotation = rotation;
	}

	/**
	 * Supposed to be called every update.
	 *
	 * @param deltaTime - Time difference since the last update.
	 * @param world     - World where the entity resides.
	 */
	public abstract void tick(float deltaTime, World world);
}
