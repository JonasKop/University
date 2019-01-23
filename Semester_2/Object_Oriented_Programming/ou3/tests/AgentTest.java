import org.junit.Test;
import static org.junit.Assert.*;
import java.util.ArrayList;
import java.util.HashMap;

public class AgentTest {

    /*@Test
    public void shouldMoveAgentAndDecreaseLifeByOne(){
        Agent test = new Agent(10,2);
        HashMap<Integer, Event> testmap = new HashMap<>();
        Event prov = new Event(new Position(1,2), 2);
        testmap.put(1,prov);
        test.setEvents(testmap);
        test.moveAgent();
        assertEquals(9,test.getStep());
    }*/
    @Test
    public void retrieveTheMapOfEvents(){
        Agent test = new Agent(10);
        assertEquals(new HashMap<>(), test.getEvents());
    }
    @Test
    public void shouldSetNewHashMapOverwritingCurrentEventsMap(){
        Agent test = new Agent(10);
        HashMap<Integer, Event> testmap = new HashMap<>();
        test.setEvents(testmap);
        assertEquals(new HashMap<>(), test.getEvents());
    }
    /*@Test
    public void checkHowManyStepsTheMessageHasGone(){
        Agent test = new Agent(10, 2);
        assertEquals(8, test.getStep());
    }*/
    @Test
    public void checkIfPositionHasBeenVisited(){
        Agent test = new Agent(10);
        Position pos = new Position(1,2);
        assertFalse(test.beenVisited(pos));
    }                  
    @Test
    public void addPositionToTheVisitedList(){
        Agent test = new Agent(10);
        Position pos = new Position(1,2);
        test.addVisited(pos);
        assertTrue(test.beenVisited(pos));
    }

}
