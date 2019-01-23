import java.util.Random;

/**
 * Class: DecideObject.
 * Objective Oriented Programming, OU3 - "Rumor Routing".
 * Created by Pontus Eriksson, dv16pen on 2017-05-04.
 * Comment: Java class that handles the chances of different events happening for all nodes in a network.
 *          If en event is triggered, then the event will be added to the node in which the event happened.
 *          Given an event there is a chance that an agent will be created and added to the node.
 */

public class DecideObject {

    public DecideObject(){

    }

    /**
     * Given the numerator and denominator, randomizes a boolean given the probability of an event happening.
     * @return boolean based on the probability of an event happening
     */
    private boolean eventTrigger(){
        Random random = new Random();
        return random.nextInt(Config.getChanceEventDen()) < Config.getChanceEventNum();

    }

    /**
     * Given the numerator and denominator, randomizes a boolean given the probability of an agent happening.
     * @return boolean based on the probability of an agent happening
     */
    private boolean agentTrigger(){
        Random random  = new Random();
        return random.nextInt(Config.getChanceAgentDen()) < Config.getChanceAgentNum();
    }

    /**
     * Updates the nodes with anny eventually events and agents and returns the updated set of nodes.
     * @param nodes the original array of nodes
     * @param time the current time
     * @return the updated array of nodes
     */
    public Node[][] eventUpdate(Node[][] nodes, int time){
        for (int i = 0; i < nodes.length; i++) {
            for (int j = 0; j < nodes[i].length; j++) {
                if(eventTrigger()){
                    nodes[i][j] = createEvent(nodes[i][j], time);
                    if (agentTrigger()){
                        nodes[i][j] = createAgent(nodes[i][j], Config.getAgentLife());
                    }
                }
            }
        }
        return nodes;
    }

    /**
     * Creates a new event and adds the event to the given node.
     * @param node the node in which the event will be saved
     * @param time the current time
     * @return the updated node
     */
    private Node createEvent(Node node, int time){
        node.addEvent(new Event(node.getPosition(), time));
        return node;
    }

    /**
     * Creates a new query. !==! Varför har vi denna? !==!
     * @param eventID the id of the query
     * @param life the life of the query
     * @return a new query
     */
    private Query createQuery(int eventID, int life){
        return new Query(eventID, life);
    }

    /**
     * Creates a new agent and adds the agent to the list of messages in the node.
     * @param node the node in which the agent will be added
     * @param life the max life of the agent
     * @return the updated node
     */
    private Node createAgent(Node node, int life){
        node.addMessage(new Agent(life));
        return node;
    }
}
