/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: Position
 * Description: Represents a position with an x and y value.
 * Author: Jonas Sjödin
 * Date: 2017-04-28
 * CS: id16jsn@cs.umu.se
 */

public class Position {
    private int x;
    private int y;

    /**
     * Constructor that sets x and y to the params.
     * @param x X position
     * @param y Y position
     * @throws IllegalArgumentException When Position is out of range
     * according to Config file
     */
    public Position(final int x, final int y){
        this.x = x;
        this.y = y;
        if (x > Config.getXMax() || y > Config.getYMax() || y < 0
                || x < 0)
            throw new IllegalArgumentException("Position is out of range");

    }

    /**
     * Returns the positions x-value.
     * @return the x-int
     */
    public int getX() {
        return x;
    }

    /**
     * Returns the positions y-value.
     * @return the y-int
     */
    public int getY() {
        return y;
    }

    /**
     * Checks if two position are equal.
     * @param o - a Position.
     * @return true if same position, else false.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (o == null || getClass() != o.getClass())
            return false;

        Position p = (Position) o;

        return (x == p.x && y == p.y);
    }
    
    /**
     * Generates a hashCode for this position.
     * @return hashcode for this position
     */
    @Override
    public int hashCode() {
        int result = x;
        result = 31 * result + y;
        return result;
    }
}
