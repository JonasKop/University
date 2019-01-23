import org.junit.Test;
import java.util.Stack;
import static org.junit.Assert.*;


public class QueryTest {

    @Test
    public void shouldGetEventID() throws Exception {
        Query test = new Query(100, 2);
        assertEquals(100, test.getEventID());
    }
    @Test
    public void shouldGetPath() throws Exception{
        Query test = new Query(100, 2);
        assertEquals(test.getPath(), new Stack<>());
    }
    @Test
    public void PushPosToPath(){
        Query test = new Query(100, 2);
        test.addToPath(new Position(1,2));
        assertEquals(new Position(1,2), test.getPath().peek());
    }
    @Test
    public void getIsReturning(){
        Query test = new Query(100, 2);
        test.returnQuery();
        assertTrue(test.getIsReturning());
    }
    @Test
    public void setMessage(){
        Query test = new Query(100, 2);
        test.setMessage("Hej");
        assertEquals("Hej", test.getMessage());
    }
}
