/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: Maze
 * Description: A simple main method that runs two robots in a maze.
 * Author: Jonas Sjödin
 * Date: 2017-04-22
 * CS: id16jsn@cs.umu.se
 */

import java.io.*;

public class Main {
    public static void main(String[] args) {

        try{
            Maze maze = new Maze(new FileReader(new File(args[0])));
            MemoryRobot mrobo = new MemoryRobot(maze);
            RightHandRuleRobot robo = new RightHandRuleRobot(maze);

            maze.printMazeRobots(robo, mrobo);

            int stepCounter = 0;

            for (int i = 0; i < 1000; i++){
                stepCounter++;
                System.out.println("Steps: " + stepCounter);
                mrobo.move();
                robo.move();
                maze.printMazeRobots(robo, mrobo);
                if (mrobo.hasReachedGoal())
                    System.out.println("MemoryRobot has reached the goal!");
                if (robo.hasReachedGoal())
                    System.out.println("RightHandRuleRobot has reached the goal!");
                if (mrobo.hasReachedGoal() && robo.hasReachedGoal())
                    break;
            }
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }
}
