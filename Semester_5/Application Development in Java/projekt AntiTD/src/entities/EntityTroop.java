package entities;

import java.util.ArrayList;

import entities.effects.Effect;
import entities.troops.Troop;
import player.Player;
import primitives.Position;
import world.World;

/**
 * Entity that represents a troop in the world. All logic for the troop go in here.
 *
 * @author Martin Hedberg, c17mhg
 */
public class EntityTroop extends Entity {

	private int health;
	private float healthToHeal = 0.f;
	private Player owner;
	private Troop troop;
	private Position velocity = new Position(0, 0);
	private Position target;
	private Position lastPos;
	private float speedMultiplier = 1.f;
	private boolean isAlive;

	/**
	 * Creates a new troop.
	 *
	 * @param troop - The troop type.
	 * @param pos   - Spawning position.
	 * @param owner - Troops owner.
	 */
	public EntityTroop(Troop troop, Position pos, Player owner) {
		super(pos);
		this.owner = owner;
		this.troop = troop;
		this.health = troop.getHealth();
		this.isAlive = true;
	}

	/**
	 * Damage the troop. If health goes under 0, kills the troop.
	 *
	 * @param damage - Damage to inflict.
	 */
	public void damage(int damage) {
		this.health -= damage;
		if (this.health <= 0) {
			this.kill();
		}
	}

	/**
	 * Get the health of the troop.
	 *
	 * @return - Health
	 */
	public int getHealth() {
		return health;
	}

	@Override
	public void tick(float deltaTime, World world) {
		if (this.isAlive) {
			if (this.inGoal(world)) {
				this.owner.addScore(1);
				this.kill();
			}

			this.runEffects(deltaTime, world);
			this.validatePosition(world);
			this.updateVelocity(world);
			this.updateTransform(deltaTime, world);
		}
	}

	/**
	 * Runs all effects that the entity is under, supposed to be run every tick.
	 *
	 * @param deltaTime - Time difference since the last update.
	 * @param world     - World where the entity resides.
	 */
	private void runEffects(float deltaTime, World world) {
		ArrayList<Effect> effects = world.getEffectsOnTile(this.getPos());
		for (Effect effect : effects) {
			effect.onTile(deltaTime, world, this);
		}
	}

	/**
	 * Runs all effects that the entity is under, once the tile enters a new tile.
	 *
	 * @param world
	 */
	private void runEffectsEnter(World world) {
		ArrayList<Effect> effects = world.getEffectsOnTile(this.getPos());
		for (Effect effect : effects) {
			effect.onEnterTile(world, this);
		}
	}

	/**
	 * Runs all effects that the entity is under, once the entity leaves the tile.
	 *
	 * @param world - World in which the entity resides.
	 * @param pos   - The position of the tile the entity is leaving.
	 */
	private void runEffectsExit(World world, Position pos) {
		ArrayList<Effect> effects = world.getEffectsOnTile(pos);
		for (Effect effect : effects) {
			effect.onExitTile(world, this);
		}
	}

	/**
	 * Validate that the tile is on a valid position.
	 *
	 * @param world - World in which the entity resides.
	 */
	private void validatePosition(World world) {
		if (lastPos == null || !lastPos.equalsRounded(this.getPos())) {
			this.troop.onEnterNewTile(this, world, this.owner);
			if (lastPos != null) {
				this.runEffectsExit(world, lastPos);
			}
			this.runEffectsEnter(world);
			this.lastPos = this.getPos();
			this.target = world.getNextRoad(this.getPos());
		}
		if (this.target == null) {
			this.kill();
			return;
		}
	}

	/**
	 * Updates the troops velocity.
	 *
	 * @param world - World in which the troop resides.
	 */
	private void updateVelocity(World world) {
		if (this.target == null) {
			return;
		}
		Position diff = Position.subtract(this.target.getCenteredPosition(),
				this.getPos());
		this.velocity = diff.normalize();
	}

	/**
	 * Updates the transformation(position and rotation) of the troop.
	 *
	 * @param deltaTime - Time between each tick.
	 */
	private void updateTransform(float deltaTime, World world) {
		this.setPos(new Position(this.getPos().getX() + (this.velocity.getX() *
				this.troop.getSpeed() * deltaTime * this.speedMultiplier),
				this.getPos().getY() + (this.velocity.getY() * this.troop.getSpeed()
						* deltaTime * this.speedMultiplier)));
		this.setRotation(this.velocity.getAngle());
	}

	/**
	 * Is the troop on a goal tile?
	 *
	 * @param world - World in which the troop resides.
	 * @return If the troop is in goal.
	 */
	private boolean inGoal(World world) {
		Position roundPos = new Position((float) Math.floor(this.getPos().getX()),
				(float) Math.floor(this.getPos().getY()));
		for (Position goal : world.getGoalPositions()) {

			if (goal.equals(roundPos)) {
				return true;
			}
		}

		return false;
	}

	/**
	 * Heals the entity by the given amount.
	 *
	 * @param health - HP to heal.
	 */
	public void heal(float health) {
		this.healthToHeal += health;
		if (this.healthToHeal >= 1) {
			this.health += Math.floor(this.healthToHeal);
			this.healthToHeal -= Math.floor(this.healthToHeal);

			if (this.health > troop.getHealth()) {
				this.health = troop.getHealth();
			}
		}
	}

	/**
	 * Mark this entity to be killed.
	 */
	public void kill() {
		this.isAlive = false;
	}

	/**
	 * Is this entity marked to be killed?
	 *
	 * @return If the entity is marked to die.
	 */
	public boolean pendingKill() {
		return !this.isAlive;
	}

	/**
	 * Get the troop that this entity represents.
	 *
	 * @return The troop
	 */
	public Troop getTroop() {
		return this.troop;
	}

	/**
	 * Sets a speed multiplier for the entity, for example 2 will make it go double
	 * speed.
	 *
	 * @param speedMultiplier - The speed multiplier
	 */
	public void setSpeedMultiplier(float speedMultiplier) {
		this.speedMultiplier = speedMultiplier;
	}

	@Override
	public String toString() {
		return this.troop.getName();
	}
}
