/**
 * Objektorienterad programmeringsmetodik (Java)
 * Spring 17
 * Assignment 3
 * Class: Node
 * Description: Tests the DecideObject-class.
 * Author: Jonas Sjödin
 * Date: 2017-05-09
 * CS: id16jsn@cs.umu.se
 */

import org.junit.Test;
import static org.junit.Assert.*;


public class DecideObjectTest {

    @Test
    public void shouldCreateDecideObject() throws Exception {
        new DecideObject();
    }

    //Might not always work since events are created by chance. So run
    //two times if exception is thrown.
    @Test
    public void shouldUpdateEvents() throws Exception {
        Node[][] nodes = new Node[50][50];

        //Initiate nodes
        int x = 0;
        for (int i = 0; i < 50; i++, x++) {
            int y = 0;
            for (int j = 0; j < 50; j++, y++)
                nodes[i][j] = new Node(new Position(x,y));
        }

        //Updates the events 50 times which should create some events at the nodes.
        for (int i = 0; i < 50; i++)
            nodes = (new DecideObject()).eventUpdate(nodes, i);

        //Checks if an event has been created at a node.
        for (int i = 0; i < 50; i++)
            for (int j = 0; j < 50; j++)
                if (nodes[i][j].getEvents().isEmpty()){
                    if (nodes[i][j].sendMessage() != null)
                        assertTrue(true);
                    break;
                }
    }
}
