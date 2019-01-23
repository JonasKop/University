/**
 * @author Wilhelm Ackermann
 * Date: 2017-05-02
 * For Objektorienterad Programmeringsmetodik OVN3
 * This class contains methods for creating messages. Although functional
 * on it's own, not very expansive.
 *
 */
public class Message {
    public int life;

    /**
     * Constructor for the message which sets the parameter "life" as the
     * amount of "life" within the class.
     *
     * @param life - interger with the amount of lives Message is supposed to
     *            have
     * @throws IllegalArgumentException When a message with no lives are
     * created.
     */
    public Message(int life) throws IllegalArgumentException {
        if (life < 0) {
            throw new IllegalArgumentException("Integer is below 0. Message " +
                    "is already dead.");
        }
        this.life = life;
    }

    /**
     * >This method asks if the message is still alive
     * @return - boolean depending whether or not the message is alive.
     */
    public boolean isAlive() {
        if (life > 0) {
            return true;
        }
        return false;
    }

    /**
     * Reduces the age of the message by one.
     */
    public void age() {
        life--;
    }

    /**
     * Returns the amount of lives as an interger
     * @return - amount of lives
     */
    public int getLife() {
        return life;
    }
}
