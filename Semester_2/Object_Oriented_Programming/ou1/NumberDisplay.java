/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: NumberDisplay
 * Description: A representation of a number display.
 * Author: Jonas Sjödin
 * Date: 2017-03-31
 * CS: id16jsn@cs.umu.se
 */


public class NumberDisplay {

    private int minLimit, maxLimit, value;

    /**
     * Constructor: creates a numberDisplay with a max-limit and a min-limit.
     *
     * @param minLimit: (int) Minimum limit for the numberDisplay
     * @param maxLimit: (int) Maximum limit for the numberDisplay
     * @throws ArithmeticException if maxLimit isn't greater than minLimit.
     */
    public NumberDisplay(int minLimit, int maxLimit) throws
            IllegalArgumentException {

        if (!(minLimit < maxLimit))
            throw new IllegalArgumentException("maxLimit must be greater than" +
                    " minLimit");

        this.minLimit = minLimit;
        this.maxLimit = maxLimit;
    }

    /**
     * Returns the value on the numberDisplay.
     * @return (int) value
     */
    public int getValue(){
        return value;
    }

    /**
     * Sets a new value on the numberDisplay.
     *
     * @param newValue: (int) The new value.
     * @throws IllegalArgumentException if nevValue isn't within range of
     * minValue and maxValue
     */
    public void setValue(int newValue) throws IllegalArgumentException {

        if (newValue < minLimit)
            throw new IllegalArgumentException("newValue is lesser than " +
                    "minLimit");
        else if (newValue > maxLimit)
            throw new IllegalArgumentException("newValue is greater than " +
                    "maxLimit");
        if (newValue == maxLimit) {
            value = minLimit;
            return;
        }
        value = newValue;
    }

    /**
     * Returns displayValue with as many characters as maxLimit. If less nr of
     * characters fill with zeros first until same nr of characters.
     *
     * @return (String) displayValue
     */
    public String getDisplayValue() {

        int valLength = String.valueOf(value).length();
        int maxLength = String.valueOf(maxLimit).length();
        String displayValue = "";

        if (valLength < maxLength)
            for (int i = 0; i < maxLength - valLength; i++) {
                displayValue = "0" + displayValue;
            }
        return displayValue + String.valueOf(value);
    }

    /**
     * Increases the value with one. If value is maxLimit it is set to minLimit.
     */
    public void increment() {

        value++;
        if (value == maxLimit)
            value = minLimit;
    }

    /**
     * Checks if the NumberDisplay did just wrap around and reset to minLImit.
     * If it did it returns true, else it returns false.
     *
     * @return (boolean)
     */
    public boolean didWrapAround() {
        return (value == minLimit);
    }
}