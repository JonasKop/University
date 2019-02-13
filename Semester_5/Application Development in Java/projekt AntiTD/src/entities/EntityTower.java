package entities;

import java.util.List;
import java.util.Random;

import primitives.Position;
import world.World;

/**
 * Entity that represents a tower in the world. All logic for towers go in here.
 *
 * @author Martin Hedberg, c17mhg
 */
public class EntityTower extends Entity {

	private Tower tower;
	private EntityTroop target;
	private float timeSinceFire = 0;
	private float fireTime = 0;
	private boolean isFiring = false;
	private final float FIRE_TIME = 0.3f;

	/**
	 * Creates a new tower.
	 *
	 * @param tower - Tower to create.
	 * @param pos   - Position of the tower.
	 */
	public EntityTower(Tower tower, Position pos) {
		super(pos);
		this.tower = tower;
		this.setRotation(new Random().nextDouble() * 2 * Math.PI - Math.PI);
	}

	@Override
	public void tick(float deltaTime, World world) {
		this.updateTarget(world);
		this.updateFireState(deltaTime);
	}

	/**
	 * Finds a new target inside the towers range.
	 *
	 * @param world
	 */
	private void findTarget(World world) {
		List<EntityTroop> troops = world.getTroops();
		double dist = tower.getRange();
		Entity foundTarget = null;
		for (int i = 0; i < troops.size(); i++) {
			EntityTroop t = troops.get(i);
			double distBetween = this.getPos().getDistanceTo(t.getPos());
			if (distBetween < dist && t.getTroop().canBeTargeted()) {
				dist = distBetween;
				foundTarget = t;
			}
		}

		if (foundTarget != null) {
			this.target = (EntityTroop) foundTarget;
		}
	}

	/**
	 * Updates the firing state of the tower.
	 *
	 * @param deltaTime - Time between each update.
	 */
	private void updateFireState(float deltaTime) {
		if (this.isFiring) {
			if (this.fireTime >= FIRE_TIME) {
				this.isFiring = false;
				this.fireTime = 0;
				this.timeSinceFire = 0;
				if (this.target != null) {
					this.target.damage(this.tower.getDamage());
				}
			}

			this.fireTime += deltaTime;
		} else {
			if (this.timeSinceFire >= this.tower.getFireRate()) {
				if (this.target != null) {
					this.isFiring = true;
				}
			}

			this.timeSinceFire += deltaTime;
		}
	}

	/**
	 * Keep tracks of the target.
	 *
	 * @param world - World in which the tower resides.
	 */
	private void updateTarget(World world) {
		if (this.target != null) {
			if (this.target.pendingKill()) {
				this.target = null;
			} else if (this.getPos().getDistanceTo(
					this.target.getPos()) >= tower.getRange()) {
				this.target = null;
			}
		}

		if (this.target == null) {
			findTarget(world);
		}

		if (target != null) {
			Position diff = Position.subtract(target.getPos(), this.getPos());
			Position normDiff = diff.normalize();
			this.setRotation(normDiff.getAngle());
		}
	}

	/**
	 * If the tower has a target, returns the position. Else null.
	 *
	 * @return The targets position if target exsist, null otherwise.
	 */
	public Position getTargetPosition() {
		if (this.target != null) {
			return target.getPos();
		}

		return null;
	}

	/**
	 * The firing state of the tower
	 *
	 * @return Fire state
	 */
	public boolean isFiring() {
		return isFiring;
	}

	/**
	 * Tower entity properties.
	 *
	 * @return A tower
	 */
	public Tower getTower() {
		return this.tower;
	}

	/**
	 * The range for shooting.
	 *
	 * @return The towers range.
	 */
	public float getRange() {
		return tower.getRange();
	}
}
