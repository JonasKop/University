/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: MemoryRobot
 * Description: A robot which uses depth-first-search (DFS) for finding its way.
 * Author: Jonas Sjödin
 * Date: 2017-04-20
 * CS: id16jsn@cs.umu.se
 */

import java.util.ArrayList;
import java.util.Stack;

public class MemoryRobot extends Robot {

    private ArrayList<Position> visited = new ArrayList<>();
    private Stack<Position> unvisitedStack = new Stack<>();

    /**
     * Constructor: Creates a memory robot with a given maze and sets the
     * starting position as visited.
     * @param maze - A given maze that the robot should find a way through
     */
    public MemoryRobot(final Maze maze){
        super(maze);
        visited.add(getCurrentPosition());
    }

    /**
     * Moves the robot by using Depth First Search (DFS) through the maze.
     */
    public void move(){
        Position p = getCurrentPosition();

        if (!hasReachedGoal()){
            if (maze.isMovable(p.getPosToNorth()) &&
                    !isVisited(p.getPosToNorth()))
                unvisitedStack.push(p.getPosToNorth());
            if (maze.isMovable(p.getPosToSouth()) &&
                    !isVisited(p.getPosToSouth()))
                unvisitedStack.push(p.getPosToSouth());
            if (maze.isMovable(p.getPosToWest()) &&
                    !isVisited(p.getPosToWest()))
                unvisitedStack.push(p.getPosToWest());
            if (maze.isMovable(p.getPosToEast()) &&
                    !isVisited(p.getPosToEast()))
                unvisitedStack.push(p.getPosToEast());

            setCurrentPosition(unvisitedStack.peek());
            visited.add(unvisitedStack.pop());
        }
    }

    /**
     * Checks if a given position is visited by the robot.
     * @param p - the position to check if visited
     * @return true if visited, else false
     */
    private boolean isVisited(final Position p){
        for (int i = 0; i < visited.size(); i++)
            if (visited.get(i).equals(p))
                return true;
        return false;
    }
}