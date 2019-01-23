/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: Node
 * Description: A node in the rumour routing algorithm-network.
 * Author: Jonas Sjödin
 * Date: 2017-05-02
 * CS: id16jsn@cs.umu.se
 */

import java.util.*;

public class Node {
    private Position position;
    private ArrayList<Position> neighbours = new ArrayList<Position>();
    private HashMap<Integer, Event> events = new HashMap<>();
    private Queue<Message> messageQueue = new LinkedList<>();
    private ArrayList<String> messages = new ArrayList<>();
    private int time = 0;


    /**
     * Constructor: Sets the nodes Position to the argument.
     * @param position - the position to set the node to
     */
    public Node(final Position position){
        this.position = position;
    }

    /**
     * Adds a position to the nodes neighbour-ArrayList.
     * @param neighPos - the position to att to the neighbour-ArrayList
     */
    public void setNeighbour(final Position neighPos){
        if (!neighbours.contains(neighPos))
            neighbours.add(neighPos);
    }

    /**
     * Adds an event to the event-hashtable.
     * @param e - the event that should be added to the hashTable.
     */
    public void addEvent(final Event e){
        events.put(e.getID(), e);
    }

    /**
     * Returns the events-hashtable.
     * @return - events-hashtable.
     */
    public HashMap<Integer, Event> getEvents(){
        return events;
    }

    /**
     * Handles a message and adds it to the message queue if it should be added.
     * @param message - the message (Query/Agent).
     */
    public void addMessage(final Message message){
        if (message instanceof Agent){
            Agent a = (Agent)message;
            events.putAll(a.getEvents());
            a.setEvents(events);
            a.addVisited(position);
        }
        else if (message instanceof Query){
            Query q = (Query)message;
            if (!q.getIsReturning()){
                if (events.get(q.getEventID()) != null) {
                    q.returnQuery();
                    messages.add(q.getMessage());
                }
                else
                    q.addToPath(position);
            }
        }
        messageQueue.add(message);
    }

    /**
     * Sends the position that the agent or query should go to next.
     * @return
     */
    public Position sendPosition(){
        if (!messageQueue.isEmpty()){
            Message message = messageQueue.element();
            if (message != null){
                if (message instanceof Agent) {
                    Agent a = (Agent) message;

                    for (int i = 0; i < neighbours.size(); i++)
                        if (!a.beenVisited(neighbours.get(i))) {
                            return neighbours.get(i);
                        }
                } else if (message instanceof Query) {
                    Query q = (Query) message;
                    if (q.getIsReturning())
                        return q.getPath().pop();

                    Stack<Position> qPositions = q.getPath();

                    while (!qPositions.isEmpty()) {
                        if (!neighbours.contains(qPositions.peek())) {
                            return qPositions.pop();
                        }
                    }
                }
                Random rand = new Random();
                return neighbours.get(rand.nextInt(neighbours.size() - 1));
            }
        }
        return null;
    }

    /**
     * Sends a message if the queue is not empty.
     * @return the top of the messageQueue.
     */
    public Message sendMessage() {

        if (!messageQueue.isEmpty())
            return messageQueue.remove();
        return null;
    }

    /**
     * Returns the nodes position.
     * @return - the nodes position
     */
    public Position getPosition() {
        return position;
    }

    /**
     * Sets the time to the argument.
     * @param t - a time int
     */
    public void setTime(final int t){
        time = t;
    }

    /**
     * Returns time.
     * @return - time
     */
    public int getTime(){
        return time;
    }
}