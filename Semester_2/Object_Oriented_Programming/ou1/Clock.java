/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: Clock
 * Description: A clock that is using a number display from class numberDisplay.
 * Author: Jonas Sjödin
 * Date: 2017-30-31
 * CS: id16jsn@cs.umu.se
 */


public class Clock {

    private NumberDisplay hours = new NumberDisplay(0,24);
    private NumberDisplay minutes = new NumberDisplay(0,60);
    private String displayString;

    /**
     * Constructor without params. Sets hours and minutes to 0.
     */
    public Clock() {

        setTime(0, 0);
        updateDisplay();
    }

    /**
     * Constructor with params: Sets hours to hour and minutes to minute.
     * @param hour:   (int) the hour that the clock should be set to
     * @param minute: (int) the minute that the clock should be set to
     */
    public Clock(int hour, int minute) {

        setTime(hour, minute);
        updateDisplay();
    }

    /**
     * Increases the minute by one. If minute reaches maxLimit of class minute
     * it resets to minLimit and hours are increased by one.
     */
    public void timeTick() {

        minutes.increment();
        if (minutes.didWrapAround())
            hours.increment();
        updateDisplay();
    }

    /**
     * Sets the time on the clock to given params.
     *
     * @param hour:   (int) the hour that the clock should be set to
     * @param minute: (int) the minute that the clock should be set to
     * @throws IllegalArgumentException if minute or hour is out of range.
     */
    public void setTime(int hour, int minute) throws IllegalArgumentException {

        hours.setValue(hour);
        minutes.setValue(minute);
        updateDisplay();
    }

    /**
     * Gets the current time of the clock in a string.
     * @return (String) displayString
     */
    public String getTime() {return displayString;}

    /**
     * Updates the displayString with [hours + : + minutes]
     */
    private void updateDisplay() {

        displayString = hours.getDisplayValue() + ":" +
                minutes.getDisplayValue();
    }
}