/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 1
 * Class: Main
 * Description: En testkörning av Klasserna NumberDisplay, Clock och AlarmClock.
 * Author: Jonas Sjödin
 * Date: 2017-03-31
 * CS: id16jsn@cs.umu.se
 */


public class Main {
    public static void main(String[] args) {

        AlarmClock clock = new AlarmClock(22,40);
        clock.setAlarm(01,40);
        System.out.println(clock.getTime());
        while(true){
            System.out.println(clock.getTime());
            if (clock.isTriggered())
		        break;
            clock.timeTick();
        }
    }
}
