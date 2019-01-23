/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: Time
 * Description: Represents a time that can count.
 * Author: Jonas Sjödin
 * Date: 2017-04-28
 * CS: id16jsn@cs.umu.se
 */
 
public class Time {
    private int time = 0;

    /**
     * Constructor doesn't do anything.
     */
    public Time(){

    }

    /**
     * Increases time by 1.
     */
    public void increment(){
        time++;
    }

    /**
     * Returns the time.
     * @return (int) time
     */
    public int getTime() {
        return time;
    }
}
