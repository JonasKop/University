import org.junit.Test;

import static org.junit.Assert.*;

public class MessageTest {
    @Test
    public void createMessage() {
        Message message = new Message(20);
    }

    @Test (expected = IllegalArgumentException.class)
    public void createIllegalMessage() {
        Message message = new Message(-1);
    }

    @Test
    public void testIsAliveWhenAbove0() {
        Message message = new Message(1);
        assertTrue(message.isAlive());
    }

    @Test
    public void decreaseLife() {
        Message message = new Message(2);
        message.age();
        message.age();
        assertFalse(message.isAlive());
    }

    @Test
    public void testThatLifeIsNotSetToZero() {
        Message message = new Message(2);
        message.age();
        assertTrue(message.isAlive());
    }

    @Test
    public void testGetLife() {
        Message message = new Message(2);
        message.age();
        assertEquals(1,message.getLife());
    }


}
