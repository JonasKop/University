/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 2
 * Class: Maze
 * Description: Tests the position class.
 * Author: Jonas Sjödin
 * Date: 2017-04-22
 * CS: id16jsn@cs.umu.se
 */

import org.junit.Test;
import static org.junit.Assert.*;

public class PositionTest {
    @Test
    public void shouldCreatePosition() throws Exception{
        new Position(1,2);
    }
    @Test
    public void shouldGetX() throws Exception{
        Position p = new Position(1,2);
        assertEquals(p.getX(), 1);
    }
    @Test
    public void shouldGetY() throws Exception{
        Position p = new Position(1,2);
        assertEquals(p.getY(), 2);
    }
    @Test
    public void shouldGetPosNorth() throws Exception{
        Position p = new Position(1,2);
        assertEquals(new Position(1,1), p.getPosToNorth());
    }
    @Test
    public void shouldGetPosSouth() throws Exception{
        Position p = new Position(1,2);
        assertEquals(new Position(1,3), p.getPosToSouth());
    }
    @Test
    public void shouldGetPosWest() throws Exception{
        Position p = new Position(1,2);
        assertEquals(new Position(0,2), p.getPosToWest());
    }
    @Test
    public void shouldGetPosEast() throws Exception{
        Position p = new Position(1,2);
        assertEquals(new Position(2,2), p.getPosToEast());
    }
    @Test
    public void shouldBeEqual() throws Exception{
        Position p = new Position(1,2);
        Position p2 = new Position(1,2);
        assertTrue(p.equals(p2));
    }
    @Test
    public void shouldNotBeEqual() throws Exception{
        Position p = new Position(1,2);
        Position p2 = new Position(1,3);
        assertFalse(p.equals(p2));
    }
    @Test
    public void shouldGetSameHashCodes() throws Exception{
        Position p = new Position(1,2);
        Position p2 = new Position(1,2);
        assertTrue((p.hashCode() == p2.hashCode()));
    }
    @Test
    public void shouldGetDifferentHashCodes() throws Exception{
        Position p = new Position(1,2);
        Position p2 = new Position(1,3);
        assertTrue((p.hashCode() != p2.hashCode()));
    }
}
