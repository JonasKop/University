package tests.player;

import org.junit.jupiter.api.Test;
import player.Player;

import static org.junit.jupiter.api.Assertions.*;

public class PlayerTest {

    @Test
    public void shouldCreatePlayer() {
        new Player();
    }

    @Test
    public void shouldGetCredits() {
        assertEquals(new Player(1234).getScore(), 1234);
        assertEquals(new Player().getScore(), 0);
    }

    @Test
    public void shouldHandleScore() {
        Player p = new Player();
        assertEquals(p.getScore(), 0);
        p.setScore(1234);
        assertEquals(p.getScore(), 1234);
    }

    @Test
    public void shouldAddCredits() {
        Player p = new Player();
        assertEquals(p.getCredits(), 0);
        p.addCredits(54);
        p.addCredits(23);
        assertEquals(p.getCredits(), 54 + 23);
    }

    @Test
    public void shouldRemoveCredits() {
        Player p = new Player();
        assertEquals(p.getCredits(), 0);
        p.addCredits(54);
        p.addCredits(23);
        assertTrue(p.removeCredits(54 + 23));
        assertEquals(p.getCredits(), 0);
    }

    @Test
    public void shouldNotRemoveCredits() {
        Player p = new Player();
        assertEquals(p.getCredits(), 0);
        p.addCredits(54);
        p.addCredits(23);
        assertFalse(p.removeCredits(54 + 24));
        assertEquals(p.getCredits(), 54 + 23);
    }

}
