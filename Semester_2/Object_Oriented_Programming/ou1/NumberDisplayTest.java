/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: NumberDisplayTest
 * Description: Tests the NumberDisplay class.
 * Author: Jonas Sjödin
 * Date: 2017-03-31
 * CS: id16jsn@cs.umu.se
 */


import org.junit.Test;
import static org.junit.Assert.*;

public class NumberDisplayTest {

    //TExc = throw Exception

    @Test
    public void shouldCreateNrDisplayMaxGreaterThanMin() throws Exception {
        new NumberDisplay(1,2);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcCreateNrDisplayMinGreaterThanMax() throws Exception {
        new NumberDisplay(2,1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcCreateNrDisplayMinEqualToMax() throws Exception {
        new NumberDisplay(1,1);
    }

    @Test
    public void shouldSetValueWithinRange() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0,2);
        numberDisplay.setValue(1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcSetValueLesserThanMinValue() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0,2);
        numberDisplay.setValue(-1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldTExcSetValueGreaterThanMaxValue() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0,2);
        numberDisplay.setValue(3);
    }

    @Test
    public void shouldSetValueEqualToMax() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 2);
        numberDisplay.setValue(2);
    }

    @Test
    public void shouldSetValueEqualToMin() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 2);
        numberDisplay.setValue(0);
    }

    @Test
    public void shouldGetValue() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 2);
        numberDisplay.setValue(1);
        assertEquals(numberDisplay.getValue(), 1);
    }

    @Test
    public void shouldGetDisplayValue10chars() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 1000000000);
        numberDisplay.setValue(123);
        assertEquals(numberDisplay.getDisplayValue(),"0000000123");
    }

    @Test
    public void shouldIncreaseValueByOne() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 4);
        numberDisplay.setValue(1);
        numberDisplay.increment();
        assertEquals(numberDisplay.getValue(), 2);
    }

    @Test
    public void shouldResetValueToMin() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 4);
        numberDisplay.setValue(3);
        numberDisplay.increment();
        assertEquals(numberDisplay.getValue(), 0);
    }

    @Test
    public void shouldWrapAround() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 4);
        numberDisplay.setValue(3);
        numberDisplay.increment();
        assertTrue(numberDisplay.didWrapAround());
    }

    @Test
    public void shouldNotWrapAround() throws Exception {
        NumberDisplay numberDisplay = new NumberDisplay(0, 4);
        numberDisplay.setValue(2);
        numberDisplay.increment();
        assertFalse(numberDisplay.didWrapAround());
    }
}