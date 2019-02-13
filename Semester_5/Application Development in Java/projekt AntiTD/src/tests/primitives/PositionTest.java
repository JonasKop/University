package tests.primitives;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;
import primitives.Position;

class PositionTest {

    @Test
    void shouldGetCorrectCoordinates() {
        Position p1 = new Position(12, 6);
        assertEquals(p1.getX(), 12);
        assertEquals(p1.getY(), 6);
    }

    @Test
    void shouldGetCorrectDistance() {
        Position p1 = new Position(12, 6);
        Position p2 = new Position(15, 10);
        assertEquals(5, p1.getDistanceTo(p2));
    }

    @Test
    void shouldSetPosition() {
        Position p1 = new Position(12, 6);

        assertEquals(12, p1.getX());
        assertEquals(6, p1.getY());
    }
    
    @Test
    void shouldNormalize() {
    	Position p = new Position(10, 0);
    	Position normp = p.normalize();
    	assertEquals(new Position(1f, 0f), normp);
    }
    
    @Test
    void testAngle0() {
    	Position p = new Position(2, 0);
    	
    	double angle = p.getAngle();
    	assertEquals(0, angle);
    }
    
    @Test
    void testAngle45() {
    	Position p = new Position(2, -2);

    	double angle = p.getAngle();
    	assertEquals(45, Math.toDegrees(angle));
    }
    
    @Test
    void testAngle90() {
    	Position p = new Position(0, -30);
    	
    	double angle = p.getAngle();
    	assertEquals(90, Math.toDegrees(angle));
    }
    
    @Test
    void testAngleMinus135() {
    	Position p = new Position(-2, 2);
    	
    	double angle = p.getAngle();
    	assertEquals(-135, Math.toDegrees(angle));
    }
}
