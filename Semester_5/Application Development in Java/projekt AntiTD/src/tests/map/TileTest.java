package tests.map;

import static org.junit.jupiter.api.Assertions.*;

import map.Tile;
import org.junit.jupiter.api.Test;
import java.util.ArrayList;

import primitives.GroundType;
import primitives.Position;

public class TileTest {

    @Test
    public void shouldGetPosition() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        Position position = sq.getPosition();
        assertEquals(position.getX(), 73);
        assertEquals(position.getY(), 42);
    }

    @Test
    public void shouldGetIncorrectPosition() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        Position position = sq.getPosition();
        assertNotEquals(position.getX(), 72);
        assertNotEquals(position.getY(), 43);
    }

    @Test
    public void shouldGetGroundTypeDesert() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertEquals(sq.getGround(), GroundType.desert);
    }

    @Test
    public void shouldNotGetGroundTypeDesert() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertNotEquals(sq.getGround(), GroundType.grass);
    }

    @Test
    public void shouldGetGround() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertEquals(sq.getGround(), GroundType.desert);
    }

    @Test
    public void shouldGetRoad() {
        ArrayList<Position> road = new ArrayList<>();
        road.add(new Position(1, 3));
        Tile sq = new Tile(73, 42, GroundType.desert, road, false, false, new ArrayList<>());
        assertTrue(sq.isRoad());
    }

    @Test
    public void shouldNotGetRoad() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertFalse(sq.isRoad());
    }

    @Test
    public void shouldBeStart() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, true, false, new ArrayList<>());
        assertTrue(sq.isStart());
    }

    @Test
    public void shouldNotBeStart() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertFalse(sq.isStart());
    }

    @Test
    public void shouldBeGoal() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, true, new ArrayList<>());
        assertTrue(sq.isGoal());
    }

    @Test
    public void shouldNotBeGoal() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertFalse(sq.isGoal());
    }

    @Test
    public void shouldHaveSameHash() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        Tile sq2 = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        System.out.println(sq.hashCode());
        System.out.println(sq2.hashCode());
        assertEquals(sq.hashCode(), sq2.hashCode());
    }

    @Test
    public void shouldHaveDifferentHash() {
        Tile sq = new Tile(73, 42, GroundType.desert, null, false, false, new ArrayList<>());
        Tile sq2 = new Tile(63, 42, GroundType.desert, null, false, false, new ArrayList<>());
        assertNotEquals(sq.hashCode(), sq2.hashCode());
    }
}
