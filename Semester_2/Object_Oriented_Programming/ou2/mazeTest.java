/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: Maze
 * Description: Tests the maze class.
 * Author: Jonas Sjödin
 * Date: 2017-05-09
 * CS: id16jsn@cs.umu.se
 */

import org.junit.Test;
import static org.junit.Assert.*;
import java.io.File;
import java.io.FileReader;

public class mazeTest {
    @Test
    public void shouldCreateMaze() throws Exception{
        new Maze(new FileReader(new File("maze.txt")));
    }
    @Test(expected = IllegalStateException.class)
    public void shouldThrowExceptionIfMazeWithTwoStarts() throws Exception{
        new Maze(new FileReader(new File("maze2s.txt")));
    }
    @Test(expected = IllegalStateException.class)
    public void shouldThrowExceptionIfMazeWithNoStarts() throws Exception{
        new Maze(new FileReader(new File("mazeNoS.txt")));
    }
    @Test(expected = IllegalStateException.class)
    public void shouldThrowExceptionIfMazeWithNoGoal() throws Exception{
        new Maze(new FileReader(new File("mazeNoG.txt")));
    }
    @Test(expected = IllegalStateException.class)
    public void shouldThrowExceptionIfMazeWithInvalidChar() throws Exception {
        new Maze(new FileReader(new File("mazeInvalidChar.txt")));
    }
    @Test
    public void wallShouldNotBeMovable() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertFalse(m.isMovable(new Position(0,0)));
    }
    @Test
    public void outOfMazeShouldNotBeMovable() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertFalse(m.isMovable(new Position(-1,-1)));
    }
    @Test
    public void blankShouldBeMovable() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertTrue(m.isMovable(new Position(1,1)));
    }
    @Test
    public void startShouldBeMovable() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertTrue(m.isMovable(new Position(1,0)));
    }
    @Test
    public void goalShouldBeMovable() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertTrue(m.isMovable(new Position(8,6)));
    }
    @Test
    public void goalShouldBeGoal() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertTrue(m.isGoal(new Position(8,6)));
    }
    @Test
    public void notGoalShouldNotBeGoal() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertFalse(m.isGoal(new Position(8,5)));
    }
    @Test
    public void shouldGetStartPos() throws Exception{
        Maze m = new Maze(new FileReader(new File("maze.txt")));
        assertTrue(m.getStartPosition().equals(new Position(1,0)));
    }
}
