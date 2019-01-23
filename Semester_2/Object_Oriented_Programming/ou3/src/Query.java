import java.util.Stack;
/**
 * @author Wilhelm Ackermann
 * Date: 2017-05-03
 * For Objektorienterad Programmeringsmetodik OVN3
 * This class is an extension of Message which makes it act like a Query
 * according to the specification.
 */
public class Query extends Message {
    private int eventID;
    private Stack<Position> path;
    private boolean isReturning = false;
    private String message;

    /**
     * This constructor sets the local eventID to the eventID in the
     * parameter, creates a message, and initialises a new Stack for path
     * @param eventID - Integer which specifies the ID
     * @param life - amount of lives the Query is supposed to have
     */
    public Query(int eventID, int life) throws IllegalArgumentException {
        super(life);
        this.eventID = eventID;
        path = new Stack<>();
    }

    /**
     * This method returns the eventId of the Query
     * @return - eventID of the Query
     */
    public int getEventID() {
        return eventID;
    }

    /**
     * This method returns the Stack path from Query
     * @return - Stack with the Positions
     */
    public Stack<Position> getPath() {
        return path;
    }

    /**
     * This method adds a Position to the Stack path
     * @param pos - Position to be added
     */
    public void addToPath(Position pos) {
        path.push(pos);
    }

    /**
     * Returns the isReturning boolean
     * @return - boolean isReturning
     */
    public boolean getIsReturning() {
        return isReturning;
    }

    /**
     * This method sets isReturning to true
     */
    public void returnQuery() {
        isReturning = true;
    }

    /**
     * This method sets the message from the parameter to Query's own
     * message
     * @param message - String which will replace the old one.
     */
    public void setMessage(String message) {
        this.message = message;
    }

    /**
     * Returns the message stored in Query
     * @return message
     */
    public String getMessage() {
        return message;
    }
}
