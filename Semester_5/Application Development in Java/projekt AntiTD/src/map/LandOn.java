package map;

import java.util.ArrayList;

import entities.effects.Effect;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: Land on tile which can be implemented. It returns
 * the effects on the current tile
 */
public interface LandOn {
	/**
	 * Returns the effects on this tile
	 *
	 * @return effects on this tile
	 */
	ArrayList<Effect> landOn();
}
