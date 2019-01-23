/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: RightHandRuleRobot
 * Description: A robot that finds it way through a maze by always following the
 * wall to the right of it.
 * Author: Jonas Sjödin
 * Date: 2017-04-19
 * CS: id16jsn@cs.umu.se
 */

public class RightHandRuleRobot extends Robot {

    private char direction = 'e';

    /**
     * Constructor: creates a RightHandRuleRobot within a given maze.
     * @param maze - The maze that the robot should be in.
     */
    public RightHandRuleRobot(final Maze maze){
        super(maze);

        Position p = getCurrentPosition();
        int counter = 0;

        while (counter < 4){
            if (rightWall(p) && freeFront(p))
                break;
            else
                turnLeft();
            counter++;
        }
        if (counter == 4)
            System.out.println("The robot is stuck");
    }

    /**
     * Moves the robot one step following its right wall.
     */
    public void move(){
        if (!hasReachedGoal()){
            Position p = getCurrentPosition();


            if (!rightWall(p)){
                turnRight();
                moveRobot(p);
            }
            else if (freeFront(p))
                moveRobot(p);
            else if (leftWall(p)){
                turn();
                moveRobot(p);
            }
            else{
                turnLeft();
                moveRobot(p);
            }
        }
    }

    /**
     * Moves the robot one step in the current direction.
     * @param p - the position of the robot
     */
    private void moveRobot(final Position p){
        switch (direction){
            case 'n':
                setCurrentPosition(p.getPosToNorth());
                break;
            case 's':
                setCurrentPosition(p.getPosToSouth());
                break;
            case 'w':
                setCurrentPosition(p.getPosToWest());
                break;
            case 'e':
                setCurrentPosition(p.getPosToEast());
                break;
        }
    }

    /**
     * Checks if a position is moveable.
     * @param p - the position of the robot
     * @return true if the position is moveable, else it returns false
     */
    private boolean checkIfMoveable(final Position p){
        return maze.isMovable(p);
    }

    /**
     * Turns the robot around 180 degrees. Sets direction to opposite.
     */
    private void turn(){
        switch (direction){
            case 'n':
                direction = 's';
                break;
            case 's':
                direction = 'n';
                break;
            case 'w':
                direction = 'e';
                break;
            case 'e':
                direction = 'w';
                break;
        }
    }

    /**
     * Sets the robots direction to left seen from its current direction.
     */
    private void turnLeft(){
        switch (direction){
            case 'n':
                direction = 'w';
                break;
            case 's':
                direction = 'e';
                break;
            case 'w':
                direction = 's';
                break;
            case 'e':
                direction = 'n';
                break;
        }
    }

    /**
     * Sets the robots direction to right seen from its current direction.
     */
    private void turnRight() {
        switch (direction){
            case 'n':
                direction = 'e';
                break;
            case 's':
                direction = 'w';
                break;
            case 'w':
                direction = 'n';
                break;
            case 'e':
                direction = 's';
                break;
        }
    }

    /**
     * Checks if the position in front of the robot is free or not and returns
     * result.
     * @param p - the position of the robot
     * @return true if position infront of robot is moveable, else false
     */
    private boolean freeFront(final Position p){
        switch (direction){
            case 'n':
                return checkIfMoveable(p.getPosToNorth());
            case 's':
                return checkIfMoveable(p.getPosToSouth());
            case 'w':
                return checkIfMoveable(p.getPosToWest());
            case 'e':
                return checkIfMoveable(p.getPosToEast());
        }
        return false;
    }

    /**
     * Checks if it is a wall on the left of the robot.
     * @param p - the position of the robot
     * @return true if position to the left of the robot is not moveable,
     * else it returns false
     */
    private boolean leftWall(final Position p){
        switch (direction){
            case 'n':
                return !checkIfMoveable(p.getPosToWest());
            case 's':
                return !checkIfMoveable(p.getPosToEast());
            case 'w':
                return !checkIfMoveable(p.getPosToSouth());
            case 'e':
                return !checkIfMoveable(p.getPosToNorth());
        }
        return false;
    }

    /**
     * Checks if it is a wall on the right of the robot.
     * @param p - the position of the robot
     * @return true if position to the right of the robot is not moveable,
     * else it returns false
     */
    private boolean rightWall(final Position p){
        switch (direction){
            case 'n':
                return !checkIfMoveable(p.getPosToEast());
            case 's':
                return !checkIfMoveable(p.getPosToWest());
            case 'w':
                return !checkIfMoveable(p.getPosToNorth());
            case 'e':
                return !checkIfMoveable(p.getPosToSouth());
        }
        return false;
    }
}

