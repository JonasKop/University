/**
 * @author Wilhelm Ackermann
 * Date: 2017-05-02
 * For Objektorienterad Programmeringsmetodik OVN3
 * This class is an extension for message in the form of an agent.
 *
 */

import java.util.ArrayList;
import java.util.HashMap;

public class Agent extends Message{
    private HashMap<Integer, Event> events;
    private ArrayList<Position> visited;

    /**
     * This constructor creates an agent with a maxLife and a life. It
     * also initialises the HashTable and the ArrayList.
     * @param life - Interger with the amount of lives.
     * @throws IllegalArgumentException if life is negative
     */
    public Agent(int life) throws IllegalArgumentException {
        super(life);
        events = new HashMap<>();
        visited = new ArrayList<>();
    }

    public void moveAgent() {
        super.age();
        for (Event s: events.values()) {
            s.incrementDistance();
        }
    }

    /**
     * This method returns the HashTable with events in them
     * @return - HashTable with events
     */
    public HashMap<Integer, Event> getEvents() {
        return events;
    }

    /**
     * This method overwrites the old HashTable with Events with a new
     * one.
     * @param events - New HashTable with Events
     */
    public void setEvents(HashMap<Integer, Event> events) {
        this.events = events;
    }

    /**
     * Adds a Position as visited to the ArrayTable
     */
    public void addVisited(Position pos) {
        if (!beenVisited(pos))
            visited.add(pos);
    }

    /**
     * Checks if a position has been visited.
     * @param pos - a position
     * @return - true if its been visited, else false.
     */
    public boolean beenVisited(Position pos) {
        return visited.contains(pos);
    }
}
