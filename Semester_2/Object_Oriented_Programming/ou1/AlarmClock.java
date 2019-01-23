/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: AlarmClock
 * Description: An Alarm clock that extends the Clock class. Adds methods for an
 * alarmclock.
 * Author: Jonas Sjödin
 * Date: 2017-03-31
 * CS: id16jsn@cs.umu.se
 */


public class AlarmClock extends Clock {
    private boolean alarmStatus = false;
    private Clock alarm = new Clock();

    /**
     * Inherits the superclass from Clock and creates a clock.
     */
    public AlarmClock() {super();}

    /**
     * Inherits the superclass from Clock and creates a clock with set hour and
     * minute.
     * @param hour: (int) the hour that the clock should be set to
     * @param minute: (int) the minute that the clock should be set to
     */
    public AlarmClock(int hour, int minute) {super(hour, minute);}

    /**
     * Sets an alarm at a given time.
     *
     * @param hour:   (int) the hour that the alarm should be set to
     * @param minute: (int) the minute that the alarm should be set to
     */
    public void setAlarm(int hour, int minute) {
        alarm.setTime(hour, minute);
        turnOn();
    }

    /**
     * Checks if the alarm is triggered. If alarmStatus is true and alarm time
     * is the same as the clock time it triggers.
     *
     * @return (boolean)
     */
    public boolean isTriggered() {

        if (alarmStatus)
            if (alarm.getTime().equals(super.getTime())) {
                System.out.println("alarm");
                return true;
            }
        return false;
    }

    /**
     * Turns the alarm on.
     */
    public void turnOn() {

        alarmStatus = true;
    }

    /**
     * Turns the alarm off.
     */
    public void turnOff() {
        alarmStatus = false;
    }
}
