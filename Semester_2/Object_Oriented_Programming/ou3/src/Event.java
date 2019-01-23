public class Event {
    private int distance = 0;
    private Position coordinate;
    private int id;
    private int time;
    private String message = "";

    /**
     * Creates a new Event with its given time and position and gives the event an individual id.
     * @param p current position
     * @param time current time
     */
    public Event(Position p, int time) throws IllegalArgumentException {
        coordinate = p;
        if (time < 0)
            throw new IllegalArgumentException("Second argument can not be " +
                    "negative");
        this.time = time;
        generateID();
        //generateMessage();
    }

    /**
     * Returns the id of the event.
     * @return the id of the event
     */
    public int getID(){
        return id;
    }


    /**
     * Returns the formatted string containing id, time and coordinates.
     * @return formatted message
     */
    private void generateMessage(){
        message = String.format("ID: %4d Time: %3d X: %3d Y: %3d\n",
                id, time, coordinate.getX(), coordinate.getY());
    }

    public String getMessage() {
        return message;
    }

    /**
     * Generates id by hashing position and current time.
     */
    private void generateID(){
        int result = coordinate != null ? coordinate.hashCode() : 0;
        id = 31 * result + time;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Event event = (Event) o;

        if (time != event.time) return false;
        return coordinate != null ? coordinate.equals(event.coordinate) : event.coordinate == null;
    }

    @Override
    public int hashCode() {
        return id;
    }

    public boolean isFinalEvent(){
        return distance == 0;
    }

    /**
     * increments distance by one.
     */
    public void incrementDistance(){
        distance++;
    }
}
