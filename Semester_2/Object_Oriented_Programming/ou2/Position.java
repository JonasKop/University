/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: Position
 * Description: This class represents a position in a 2d plane.
 * Author: Jonas Sjödin
 * Date: 2017-04-04
 * CS: id16jsn@cs.umu.se
 */

public class Position {
    private int y;
    private int x;

    /**
     * Constructor: Creates a position with given x and y coordinates.
     * @param x - x-coordinate
     * @param y - y-coordinate
     */
    public Position(final int x, final int y){
        this.x = x;
        this.y = y;
    }

    /**
     * Gets the x-position
     * @return x-position
     */
    public int getX() {return x;}

    /**
     * Gets the y-position
     * @return y-position
     */
    public int getY() {return y;}

    /**
     * Gets the position in the south of this position.
     * @return position in the south of this position
     */
    public Position getPosToSouth(){
        return new Position(x, y + 1);
    }

    /**
     * Gets the position in the north of this position.
     * @return position in the north of this position
     */
    public Position getPosToNorth(){
        return new Position(x, y - 1);
    }

    /**
     * Gets the position in the west of this position.
     * @return position in the west of this position
     */
    public Position getPosToWest(){return new Position(x - 1, y);}

    /**
     * Gets the position in the east of this position.
     * @return position in the east of this position
     */
    public Position getPosToEast(){return new Position(x + 1, y);}

    /**
     * Checks if two position classes are equal.
     * @param o - a position
     * @return true if same position, else false
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Position position = (Position) o;

        if (y != position.y) return false;
        return x == position.x;
    }

    /**
     * Hashcodes a position.
     * @return a hashcode of a position
     */
    @Override
    public int hashCode() {
        int result = y;
        result = 31 * result + x;
        return result;
    }

    public void printPos(){
        System.out.println("X:" + x + " Y:" + y);
    }
}
