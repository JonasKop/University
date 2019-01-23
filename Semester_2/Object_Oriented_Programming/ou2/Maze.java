/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: Maze
 * Description: A maze saved as an arraylist.
 * Author: Jonas Sjödin
 * Date: 2017-05-09
 * CS: id16jsn@cs.umu.se
 */


import java.io.*;
import java.util.ArrayList;

public class Maze {
    private ArrayList<ArrayList<Character>> mazeData = new ArrayList<>();
    private Position sPos;

    /**
     * Constructor: Reades a maze from a given file.
     *
     * @param mazeData - the source of the given maze
     * @throws IllegalArgumentException if wrong format on maze. Wrong amount
     *                                  of 'S' and 'G'.
     */
    public Maze(final Reader mazeData) throws Exception {
        BufferedReader reader;

        reader = new BufferedReader(mazeData);

        String line;
        int i = 0;
        int sCounter = 0;
        int gCounter = 0;
        while ((line = reader.readLine()) != null) {
            ArrayList<Character> tempMaze = new ArrayList<>();
            for (int j = 0; j < line.length(); j++) {
                Character c = line.charAt(j);
                boolean validChar = false;

                if (c == 'S' || c == 'G' || c == '*' || c == ' ')
                    validChar = true;

                if (!validChar)
                    throw new IllegalStateException("An invalid " +
                            "character has been encountered.");

                if (c == 'S') {
                    sPos = new Position(j, i);
                    sCounter++;
                }
                if (c == 'G')
                    gCounter++;
                tempMaze.add(c);
            }
            this.mazeData.add(tempMaze);
            i++;
        }
        if (sCounter != 1)
            throw new IllegalStateException("Starting position missing" +
                    " or there are too many (S) in the maze. There can " +
                    "only be one.");
        if (gCounter < 1)
            throw new IllegalStateException("Missing goal position.");
        reader.close();
    }

    /**
     * checks if a position in the maze is movable.
     * @param p - the position to check if it is movable
     * @return true if movable to, else false
     */
    public boolean isMovable(Position p){
        try{
            return (mazeData.get(p.getY()).get(p.getX()) != '*');
        }
        catch (IndexOutOfBoundsException e){
            return false;
        }
    }

    /**
     * Checks if a given position is the goal-position of a maze.
     * @param p - the position to check if it is the goal
     * @return true if it is the goal, else false.
     */
    public boolean isGoal(Position p){
        try{
            return (mazeData.get(p.getY()).get(p.getX()) == 'G');
        }
        catch (IndexOutOfBoundsException e){
            return false;
        }
    }

    /**
     * Gets the start position of the maze.
     * @return the start position
     */
    public Position getStartPosition(){
        return sPos;
    }

    /**
     * Prints two robots in a maze. Exists for easier graphical viewing.
     * @param robo - a robot that should be drawn in the maze.
     * @param robo2 - a robot that should be drawn in the maze.
     */
    public void printMazeRobots(Robot robo, Robot robo2){
        Position roboPos = robo.getCurrentPosition();
        Position robo2Pos = robo2.getCurrentPosition();
        for (int i = 0; i < mazeData.size(); i++){
            for (int j = 0; j < mazeData.get(i).size(); j++){
                if (i == roboPos.getY() && j == roboPos.getX())
                    System.out.print("R");
                else if (i == robo2Pos.getY() && j == robo2Pos.getX())
                    System.out.print("M");
                else
                    System.out.print(mazeData.get(i).get(j));
            }
            System.out.println();
        }
        System.out.println("\n");
    }

}
