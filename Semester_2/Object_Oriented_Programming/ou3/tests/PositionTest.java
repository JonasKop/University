/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: Position
 * Description: Tests the Position class.
 * Author: Jonas Sjödin
 * Date: 2017-04-30
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
    public void shouldGetSameXAsConstructorSet() throws Exception {
        assertEquals((new Position(1, 2)).getX(), 1);
    }

    @Test
    public void shouldGetSameYConstructorSet() throws Exception {
        assertEquals((new Position(1, 2)).getY(), 2);
    }

    @Test
    public void shouldBeEqual() throws Exception {
        assertTrue((new Position(1,2).equals(new Position(1,2))));
    }

    @Test
    public void shouldNotBeEqual() throws Exception {
        assertFalse((new Position(1,2).equals(new Position(1,3))));
    }

    @Test
    public void shouldGetSameHashCode() throws Exception {
        Position p = new Position(1,2);
        assertEquals(p.hashCode(), (new Position(1,2).hashCode()));
    }

    @Test
    public void shouldGetDifferentHashCode() throws Exception {
        Position p = new Position(1,3);
        assertNotEquals(p.hashCode(), (new Position(1,2).hashCode()));
    }
}
