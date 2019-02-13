package tests.world;

import entities.*;
import entities.effects.EffectSpeed;
import entities.troops.Troop;
import map.*;
import org.junit.Before;
import org.junit.Test;
import player.Player;
import primitives.Position;
import world.World;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * @author Petter Skogh (id16psh)
 * @version 1.0
 * Email: id16psh@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Description: Test for World
 */
public class WorldTest {

    private World world;
    private GameMap map;
    private Player player;
    private int standardCredits = 100000000;

    @Before
    public void setUp(){
        try {

             map = ParseMap.parse("levels.xml").get(0);
             player = new Player(standardCredits);
             world = new World(map, player);
        } catch (MapException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void shouldCreateWorld() {
       new World(map, player);
    }

    @Test
    public void shouldGetStartPositionsFromMap() {

        int correctPositions = 0;
        for(int i = 0; i < world.getStartPositions().size(); i++){
            if(world.getStartPositions().get(i).equals(map.getStarts().get(i))){
                correctPositions++;
            }
        }
        assertEquals(world.getStartPositions().size(), correctPositions);
    }

    @Test
    public void shouldGetGoalPositionsFromMap() {
        int correctPositions = 0;
        for(int i = 0; i < world.getGoalPositions().size(); i++){
            if(world.getGoalPositions().get(i).equals(map.getGoals().get(i))){
                correctPositions++;
            }
        }
        assertEquals(world.getGoalPositions().size(), correctPositions);
    }

    @Test
    public void shouldSetStartPositionAsFirstStartPositionInArray(){
        assertTrue(world.getStartPosition().equals(
                world.getStartPositions().get(0)));
    }

    @Test
    public void towerPositionsShouldBeInRangeOfRoad(){

        int nrOfTowers = 10;

        for(int i = 0; i < nrOfTowers; i++){
            Tower t = new Tower(10);
            t.setRange(2);
            world.spawnTower(t);
        }

        List<EntityTower> towers = world.getTowers();

        ArrayList<Tile> road = world.getRoad();
        int nrOfTowersInRangeOfRoad = 0;
        for(EntityTower tower : towers){

            boolean inRange = false;
            for(Tile roadTile : road){
                if(tower.getPos().getDistanceTo(roadTile.getPosition()) <
                        tower.getRange()){
                    inRange = true;
                }
            }

            if(inRange){
                nrOfTowersInRangeOfRoad++;
            }
        }

        assertEquals(nrOfTowersInRangeOfRoad, nrOfTowers);
    }

    @Test
    public void shouldNotHaveTroops(){
        assertTrue(world.getTroops().isEmpty());
    }

    @Test
    public void shouldSpawnTroop(){
        Troop troop = new Troop(10);
        world.spawnTroop(troop);

        assertEquals(1, world.getTroops().size());
    }

    @Test
    public void shouldNotSpawnTroopIfPlayerHasNoMoney() {

        player.removeCredits(player.getCredits());
        Troop troop = new Troop(10);
        world.spawnTroop(troop);

        assertEquals(0, world.getTroops().size());
    }

    @Test
    public void shouldSpawnTroopToStandardStartPosition(){

        Troop troop = new Troop(10);
        world.spawnTroop(troop);

        Position troopPos = world.getTroops().get(0).getPos();
        assertEquals(troopPos, world.getStartPositions().get(0));
    }

    @Test
    public void shouldSetStartPosition(){

        Position standardPos = world.getStartPosition();
        world.setStartPosition(map.getTile(0, 4));
        assertNotEquals(standardPos, world.getStartPosition());
    }

    @Test
    public void shouldRemoveKilledTroop(){
        Troop troop = new Troop(10);
        world.spawnTroop(troop);
        world.getTroops().get(0).kill();
        world.updateGame(20);

        assertEquals(0, world.getTroops().size());
    }

    @Test
    public void shouldAddCredits(){

        world.updateGame(20);

        assertTrue(player.getCredits() > standardCredits);
    }

    @Test
    public void shouldAddMoreCreditsIfBankerExist(){

        world.updateGame(20);
        int creditsWithoutBanker = player.getCredits();
        player.removeCredits(player.getCredits());
        player.addCredits(standardCredits);

        Troop troop = new Troop(10);
        troop.setName("Banker");
        world.spawnTroop(troop);
        world.updateGame(20);
        int creditsWithBanker = player.getCredits();

        assertTrue(creditsWithBanker > creditsWithoutBanker);
    }

    @Test
    public void shouldTickTroop(){

        Troop troop = new Troop(10);
        world.spawnTroop(troop);

        world.updateGame(20);
        assertFalse(world.getTroops().get(0).getPos().equals(
                world.getStartPosition()));
    }

    @Test
    public void shouldSetEffectOnTile(){
        EffectSpeed effect = new EffectSpeed("speed");
        Tile tile = world.getRoad().get(0);
        world.setEffect(tile.getPosition(), effect);
        assertEquals(1, tile.landOn().size());
    }

    @Test
    public void shouldGetEffectOfTile(){
        EffectSpeed effect = new EffectSpeed("speed");
        Tile tile = world.getRoad().get(0);
        world.setEffect(tile.getPosition(), effect);
        assertEquals(1, world.getEffectsOnTile(tile.getPosition()).
                size());
    }

    @Test
    public void correctRoadTilesPositionShouldBeInWorldsRoadArray(){

        Position roadTilePos = map.getTile(2,3).getPosition();

        boolean isInRoad = false;
        for(Tile t : world.getRoad()){
            if(roadTilePos.equals(t.getPosition())){
                isInRoad = true;
            }
        }

        assertTrue(isInRoad);
    }

    @Test
    public void shouldGetOneNextTargetFromPosition(){

        ArrayList<Position> nextTargets =
                world.getNextTargets(new Position(0, 1));
        assertEquals(nextTargets.size(), 1);
    }

    @Test
    public void shouldGetTwoNextTargetsFromPositionWithTwoRoads(){

        ArrayList<Position> nextTargets =
                world.getNextTargets(new Position(2, 1));
        assertEquals(nextTargets.size(), 2);
    }

    @Test
    public void allTilesInRoadShouldBeMarkedAsRoad(){

    	//TODO Only need t.isRoad()?
        ArrayList<Tile> road = world.getRoad();
        int nrOfCorrectRoadTiles = 0;
        for(Tile t : road){
            if(map.getTile(t.getPosition().getX(), t.getPosition().getY()).
                    isRoad()){
                nrOfCorrectRoadTiles++;
            }
        }

        assertEquals(road.size(), nrOfCorrectRoadTiles);
    }


}
