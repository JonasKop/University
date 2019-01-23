/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: Robot
 * Description: A generic robot-class that contains some properties that all
 * robots should have.
 * Author: Jonas Sjödin
 * Date: 2017-04-19
 * CS: id16jsn@cs.umu.se
 */

public abstract class Robot {
    private Position position;
    protected Maze maze;

    /**
     * Constructor: Creates a robot in a given maze.
     * @param maze - the given maze that the robot should be in.
     */
    public Robot(final Maze maze){
        this.maze = maze;
        this.position = maze.getStartPosition();
    }

    /**
     * An abstract generic move method that any robot-class that inherits from
     * this class has to have.
     */
    public abstract void move();

    /**
     * Gets the robots current position.
     * @return the position of the robot.
     */
    public Position getCurrentPosition()
    {
        return position;
    }

    /**
     * Sets a position for the robot.
     * @param position - a given position
     */
    protected void setCurrentPosition(final Position position){

        this.position = position;
    }

    /**
     * Checks if the robot has reached the goal in the maze and returns true if
     * it has, else false.
     * @return
     */
    public boolean hasReachedGoal(){

        return maze.isGoal(position);
    }
}
