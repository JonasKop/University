package tests.map;

import static org.junit.jupiter.api.Assertions.*;

import map.GameMap;
import map.MapException;
import map.ParseMap;
import org.junit.jupiter.api.Test;
import primitives.Position;

import java.util.ArrayList;


public class ParseMapTest {

    @Test
    public void shouldCreateMap() {
        try {
            ParseMap.parse("src/tests/map/correct_map.xml");
        } catch (MapException e) {
            fail(e);
        }
    }

    @Test
    public void shouldGetGameMaps() {
        try {
            ArrayList<GameMap> gameMaps = ParseMap.parse("src/tests/map/correct_map.xml");
            assertEquals(gameMaps.size(), 1);
        } catch (MapException e) {
            fail(e);
        }
    }

    @Test
    public void shouldGetCorrectParams() {
        try {
            ArrayList<GameMap> gameMaps = ParseMap.parse("src/tests/map/correct_map.xml");
            assertNotNull(gameMaps);
            assertNotEquals(0, gameMaps.size());
            GameMap gm = gameMaps.get(0);
            assertEquals(gm.getHeight(), 10);
            assertEquals(gm.getWidth(), 10);
            Position starts = gm.getStarts().get(0);
            Position goals = gm.getGoals().get(0);
            assertEquals(starts.getX(), 0);
            assertEquals(starts.getY(), 1);

            assertEquals(goals.getX(), 9);
            assertEquals(goals.getY(), 1);
            assertEquals(gm.getName(), "Basic gameMap");
        } catch (MapException e) {
            fail(e);
        }
    }

    @Test
    public void noSquaresShouldBeNull() {
        try {

            GameMap gm = ParseMap.parse("src/tests/map/correct_map.xml").get(0);
            int height = 10, width = 10;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    assertNotNull(gm.getTile(i, j));
                }
            }
        } catch (MapException e) {
            fail(e);
        }
    }

    @Test
    public void shouldNotCreateMap() {
        try {
            ParseMap.parse("src/tests/map/incorrect_map.xml");
            fail("");
        } catch (MapException e) {

        }
    }

    @Test
    public void shouldThrowExceptionWithIncorrectSize() {
        try {
            ParseMap.parse("src/tests/map/incorrect_size.xml");
            fail("");
        } catch (MapException e) {

        }
    }
}
