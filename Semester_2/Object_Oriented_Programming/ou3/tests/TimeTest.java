/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: Position
 * Description: Tests the Time class.
 * Author: Jonas Sjödin
 * Date: 2017-04-30
 * CS: id16jsn@cs.umu.se
 */


import org.junit.Test;
import static org.junit.Assert.*;

public class TimeTest {

    @Test
    public void shouldCreateTime() throws Exception {
        new Time();
    }

    @Test
    public void shouldGetTime0() throws Exception {
        Time t = new Time();
        assertEquals(t.getTime(), 0);
    }

    @Test
    public void shouldIncrementAndGetTime1() throws Exception {
        Time t = new Time();
        t.increment();
        assertEquals(t.getTime(), 1);
    }
}
