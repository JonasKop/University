import java.util.Random;

/**
 * Created by dv16pen on 2017-05-11.
 */
public class Randomizer extends Random{
    private Random random;

    public Randomizer(){
        random = new Random();
    }

    /**
     * Given the numerator and denominator, randomizes a boolean given the probability of an event happening.
     * @return boolean based on the probability of an event happening
     */
    public boolean eventTrigger(){
        return random.nextInt(Config.getChanceEventDen()) < Config.getChanceEventNum();

    }

    /**
     * Given the numerator and denominator, randomizes a boolean given the probability of an agent happening.
     * @return boolean based on the probability of an agent happening
     */
    public boolean agentTrigger(){
        return random.nextInt(Config.getChanceAgentDen()) < Config.getChanceAgentNum();
    }
}
