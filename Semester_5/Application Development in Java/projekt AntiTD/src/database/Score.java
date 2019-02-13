package database;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: Score class containing a name and a score which can only be set
 * at initialization and can afterwords be gotten.
 */
public class Score {

	private String name;
	private int score;

	/**
	 * Sets the name and score of the score
	 *
	 * @param name  name of the player
	 * @param score score of the player
	 */
	public Score(String name, int score) {
		this.name = name;
		this.score = score;
	}

	/**
	 * Returns the score
	 *
	 * @return the score
	 */
	public int getScore() {
		return score;
	}

	/**
	 * Returns the name
	 *
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * For easy debugging this exists, which prints the name and score.
	 *
	 * @return a string value containing name and score.
	 */
	@Override
	public String toString() {
		return "name: " + name + ", score: " + score;
	}
}
