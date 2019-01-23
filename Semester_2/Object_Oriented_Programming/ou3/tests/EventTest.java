import org.junit.Test;
import static org.junit.Assert.*;

public class EventTest {

    @Test (expected = IllegalArgumentException.class)
    public void negativeTime() {
        Position pos = new Position(1,1);
        int time = -5;
        Event event = new Event(pos, time);
    }

    @Test (expected = IllegalArgumentException.class)
    public void negativePosition() {
        Position pos = new Position(-1,-5);
        int time = 1;
        Event event = new Event(pos, time);
    }

    @Test
    public void falseIDCheck() {
        Position pos = new Position(5,1);
        Event event = new Event(pos, 1);
        assertNotNull(event.getID());
    }

    @Test
    public void atEventSpotWhenCreated() {
        Position pos = new Position(5,1);
        Event event = new Event(pos, 1);
        assertTrue(event.isFinalEvent());
    }

    @Test
    public void movingAwaySimulator() {
        Position pos = new Position(5,1);
        Event event = new Event(pos, 1);
        event.incrementDistance();
        assertFalse(event.isFinalEvent());
    }

    @Test
    public void isIDSameAsHashCode() {
        Position pos = new Position(5,1);
        Event event = new Event(pos, 1);
        assertEquals(event.hashCode(), event.getID());
    }
}
