package primitives;

import java.util.Objects;

/**
 * @author Martin Hedberg (c17mhg), Petter Skogh (id16psh)
 * @version 1.0
 * Email: c17mhg@cs.umu.se, id16psh@cs.umu.se
 * Date: December 2018
 * <p>
 * Description: Position with x and y
 */
public class Position {

	private float x;
	private float y;

	/**
	 * Constructor. Sets this x and y to given x and y.
	 *
	 * @param x - x position
	 * @param y - y position
	 */
	public Position(float x, float y) {
		this.x = x;
		this.y = y;
	}

	/**
	 * @return x
	 */
	public float getX() {
		return x;
	}

	/**
	 * @return y
	 */
	public float getY() {
		return y;
	}

	/**
	 * Measures the distance between this position and another position using
	 * pythagoras
	 *
	 * @param p - The position to check the distance to
	 * @return The distance as float
	 */
	public float getDistanceTo(Position p) {
		float distX = this.x - p.x;
		float distY = this.y - p.y;
		return (float) Math.sqrt((distX * distX) + (distY * distY));
	}

	/**
	 * @return The hashcode for the position
	 */
	@Override
	public int hashCode() {
		return Objects.hash((int) x, (int) y);
	}

	/**
	 * @return The normalized position of this position
	 */
	public Position normalize() {
		Position ret = new Position(this.x, this.y);
		float len = ret.getDistanceTo(new Position(0, 0));
		ret.x /= len;
		ret.y /= len;
		return ret;
	}

	/**
	 * The standard position is in the upper left corner of an object. This
	 * method is used to get the centered position.
	 *
	 * @return The centered position
	 */
	public Position getCenteredPosition() {
		return new Position(this.x + 0.5f, this.y + 0.5f);
	}

	/**
	 * Adds position p1 to position p2.
	 *
	 * @param p1 - Position
	 * @param p2 - Position
	 * @return The new position with p1 and p2 added to together
	 */
	public static Position add(Position p1, Position p2) {
		return new Position(p1.x + p2.x, p1.y + p2.y);
	}

	/**
	 * Subtracts position p2 from position p1.
	 *
	 * @param p1 - Position
	 * @param p2 - Position
	 * @return The new position with p2 subtracted from p1
	 */
	public static Position subtract(Position p1, Position p2) {
		return new Position(p1.x - p2.x, p1.y - p2.y);
	}

	/**
	 * Multiplies given position p1 with given float l.
	 *
	 * @param p1 - Position
	 * @param l  - Float for p1 to be multiplied with
	 * @return The new position with p1 multiplied with l
	 */
	public static Position multiply(Position p1, float l) {
		return new Position(p1.x * l, p1.y * l);
	}

	/**
	 * Calculates the angle that position is in relation to position (0, 0).
	 *
	 * @return The angle
	 */
	public double getAngle() {
		Position p = new Position(0, 0);
		return Math.atan2(p.y - this.y, this.x - p.y);
	}

	/**
	 * Compares if this position is equal given position.
	 *
	 * @param obj - The position to be compared to
	 * @return True if equal
	 */
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;

		Position p = (Position) obj;

		return Float.compare(x, p.x) == 0 && Float.compare(y, p.y) == 0;
	}

	/**
	 * Compares if this position(rounded) is equal given position(rounded).
	 *
	 * @param pos - The position to be compared to
	 * @return True if equal
	 */
	public boolean equalsRounded(Position pos) {
		return Math.floor(pos.getX()) == Math.floor(this.x) &&
				Math.floor(pos.getY()) == Math.floor(this.y);
	}

	/**
	 * @return Position as string
	 */
	@Override
	public String toString() {
		return "Position [x=" + x + ", y=" + y + "]";
	}
}
