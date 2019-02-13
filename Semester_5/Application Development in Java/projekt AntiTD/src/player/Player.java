package player;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: PlayerTest class where the name of the player is set in its
 * constructor. Its score can then be set and more credits can be added through
 * a method. All local variable can be gotten through getter methods.
 */
public class Player {
	private int score = 0;
	private double credits = 0;


	/**
	 * Empty constructor
	 */
	public Player() {
	}

	/**
	 * Sets a start credit value of the player
	 *
	 * @param credits the credits that should be set to the player.
	 */
	public Player(int credits) {
		this.credits = credits;
	}

	/**
	 * Sets the score of the PlayerTest
	 *
	 * @param score the score of the player
	 */
	public void setScore(int score) {
		this.score = score;
	}

	//TODO Hur ska vi scorea
	public void addScore(int score) {
		this.score += score;
	}

	/**
	 * Adds credits taken as parameter to its private credits variable.
	 *
	 * @param credits the nr of credits which should be added
	 */
	public void addCredits(double credits) {
		this.credits += credits;
	}

	/**
	 * Remove credits taken as parameter from its local credits method. If it
	 * does not have the amount of credits which should be removed, nothing is
	 * removed and it returns false. If true is returned it could remove the
	 * credits from the PlayerTest.
	 *
	 * @param credits the credits which should be removed
	 * @return true if success, else false.
	 */
	public Boolean removeCredits(int credits) {
		if (this.credits >= credits) {
			this.credits -= credits;
			return true;
		}
		return false;
	}

	/**
	 * Returns the credits
	 *
	 * @return the credits
	 */
	public int getCredits() {
		return (int) credits;
	}

	/**
	 * Gets the Score of the player
	 *
	 * @return the score of the player
	 */
	public int getScore() {
		return score;
	}

}
