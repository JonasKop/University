/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: AlarmClockTest
 * Description: Tests the AlarmClock class.
 * Author: Jonas Sjödin
 * Date: 2017-03-31
 * CS: id16jsn@cs.umu.se
 */


import org.junit.Test;
import static org.junit.Assert.*;

public class AlarmClockTest{

    @Test
    public void shouldCreateAlarmClockWithoutParamsTime0000() throws Exception {
        AlarmClock alarm = new AlarmClock();
        assertEquals(alarm.getTime(), "00:00");
    }

    @Test
    public void shouldCreateAlarmClockWithParamsTime0512() throws Exception {
        AlarmClock alarm = new AlarmClock(5,12);
        assertEquals(alarm.getTime(), "05:12");
    }

    @Test
    public void shouldSetAlarmAndTrigger() throws Exception {
        AlarmClock alarm = new AlarmClock(12,24);
        alarm.setAlarm(12,24);
        assertTrue(alarm.isTriggered());
    }

    @Test
    public void shouldTriggerWhenAlarmStatusFalseThenTrue() throws Exception {
        AlarmClock alarm = new AlarmClock(12,24);
        alarm.setAlarm(12,24);
        alarm.turnOff();
        alarm.turnOn();
        assertTrue(alarm.isTriggered());
    }

    @Test
    public void shouldNotTriggerWhenAlarmStatusFalse() throws Exception {
        AlarmClock alarm = new AlarmClock(12,24);
        alarm.setAlarm(12,24);
        alarm.turnOff();
        assertFalse(alarm.isTriggered());
    }
}