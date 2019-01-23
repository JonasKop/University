/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: NodeMaster
 * Description: Creates all the nodes and handles them. It also updates the
 * nodes different messages.
 * Author: Jonas Sjödin
 * Date: 2017-05-03
 * CS: id16jsn@cs.umu.se
 */

import java.util.ArrayList;
import java.util.Random;
import java.lang.Math;

public class Network {
    private static final int lenBetwNodes = 10;
    private static final int nodeRange = 15;
    private int xMax;
    private int yMax;
    private Node[][] nodes;

    /**
     * Constructor: Sets yMax, xMax and nodeSize.
     */
    public Network(){
        this.xMax = Config.getXMax();
        this.yMax = Config.getYMax();
        nodes = new Node[xMax][yMax];
        initiateNodes();
    }

    /**
     * Creates new nodes on all positions and sets their neighbours.
     */
    private void initiateNodes(){
        for (int i = 0; i < xMax; i++)
            for (int j = 0; j < yMax; j++)
                nodes[i][j] = new Node(new Position(i, j));
        setNeighbours();
    }


    /**
     * Sets the neighbours to all the nodes.
     */
    private void setNeighbours(){
        for (int i = 0; i < xMax; i++) {
            for (int j = 0; j < yMax; j++) {
                nodes[i][j] = getNeighbours(nodes[i][j]);
            }
        }
    }

    /**
     * Given a node, checks for its neighbours around the field.
     * @param nodeOrigin node of origin
     * @return returns the updated node with its neighbours
     */
    private Node getNeighbours(Node nodeOrigin){
        for (int i = 0; i < xMax; i++) {
            for (int j = 0; j < yMax; j++) {
                if (!nodes[i][j].equals(nodeOrigin) && isNeighbour(nodeOrigin, nodes[i][j])){
                    nodeOrigin.setNeighbour(new Position(i, j));
                }
            }
        }
        return nodeOrigin;
    }

    /**
     * Checks if two nodes are neighbours withing a given range.
     * @param nodeOrigin node of origin
     * @param nodeToBeTested  node to be tested
     * @return returns boolean based on if the two nodes are within range
     */
    private boolean isNeighbour(Node nodeOrigin, Node nodeToBeTested){
        double xOrigin = nodeOrigin.getPosition().getX() * lenBetwNodes;
        double yOrigin = nodeOrigin.getPosition().getY() * lenBetwNodes;
        double xTested = nodeToBeTested.getPosition().getX() * lenBetwNodes;
        double yTested = nodeToBeTested.getPosition().getY() * lenBetwNodes;
        return Math.sqrt((xOrigin - xTested) * (xOrigin - xTested) + (yOrigin - yTested) * (yOrigin - yTested)) < 15;
    }

    /**
     * Returns the current state of the node-array.
     * @return nodes-array
     */
    public Node[][] getNodes(){
        return nodes;
    }

    /**
     * Runs eventUpdate and may create events in nodes.
     * @param time - the time to set to a newly created event.
     */
    public void createRandomEvent(final int time) {
        DecideObject decideObject = new DecideObject();
        nodes = decideObject.eventUpdate(nodes, time);
    }

    /**
     * Updates all nodes messages and sends the if not busy and after all
     * updated it sets all nodes to not busy.
     */
    public void messageUpdate(final int time){
        sendObject(time);
    }

    /**
     * Updates all nodes messages and sends them if both nodes are not busy.
     */
    private void sendObject(final int time) {
        for (int i = 0; i < xMax; i++) {
            for (int j = 0; j < yMax; j++) {
                Position p = nodes[i][j].sendPosition();
                if (p != null && nodes[i][j].getTime() != time && nodes[p.getX()][p.getY()].getTime() != time) {
                    Message message = nodes[i][j].sendMessage();
                    if (message != null) {
                        nodes[p.getX()][p.getY()].addMessage(message);
                        nodes[i][j].setTime(time);
                        nodes[p.getX()][p.getY()].setTime(time);
                    }
                }
            }
        }
    }

    /**
     * Creates one event on 4 random different nodes.
     * @param time - time to set to event.
     */
    public void create4RandEvents(final int time){
        int ammountRandoms = 4;

        DecideObject decideObject = new DecideObject();
        Random rand = new Random();
        ArrayList<Position> positions = new ArrayList<>();
        int counter = 0;
        while (counter < ammountRandoms) {
            Position p = new Position(rand.nextInt(xMax - 1), rand.nextInt(yMax - 1));
            if (!positions.contains(p)) {
                positions.add(p);
                counter++;
            }
        }
        for (int i = 0; i < ammountRandoms; i++) {
            Position p = positions.get(i);
            nodes[p.getX()][p.getY()].addEvent(new Event(p, time));
        }
    }
}
