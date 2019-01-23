/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: ClockTest
 * Description: Tests the Clock class.
 * Author: Jonas Sjödin
 * Date: 2017-03-31
 * CS: id16jsn@cs.umu.se
 */


import org.junit.Test;
import static org.junit.Assert.*;

public class ClockTest {

    //TExc = throw Exception

    @Test
    public void shouldCreateClockNoParamsAndSetTimeTo0000() throws Exception {
        Clock clock = new Clock();
        assertEquals(clock.getTime(), "00:00");
    }

    @Test
    public void shouldCreateClockWithParamsAndSetTimeTo0120() throws Exception {
        Clock clock = new Clock(1,20);
        assertEquals(clock.getTime(), "01:20");
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcWhenHourTooSmall() throws Exception {
        new Clock(-1, 0);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcWhenHourTooGreat() throws Exception {
        new Clock(25,0);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcWhenMinuteTooSmall() throws Exception {
        new Clock(0, -1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcWhenMinuteTooGreat() throws Exception {
        new Clock(0,61);
    }

    @Test
    public void shouldTimeTickOneMinute() throws Exception {
        Clock clock = new Clock();
        clock.timeTick();
        assertEquals(clock.getTime(), "00:01");
    }

    @Test
    public void shouldChangeHourWhenTimeTickLastMinute() throws Exception {
        Clock clock = new Clock(12,59);
        clock.timeTick();
        assertEquals(clock.getTime(), "13:00");
    }

    @Test
    public void shouldResetWhenTimeTickLastMinuteLastHour() throws Exception {
        Clock clock = new Clock(23,59);
        clock.timeTick();
        assertEquals(clock.getTime(), "00:00");
    }
}