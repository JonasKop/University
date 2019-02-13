package antitd.eventHandler;

import world.World;
import antitd.AntiTD;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * *****************************************************************************
 * Description: Handles a tile being pressed. If pressed it runs
 * onTileClicked() on that square.
 */
public class TilePressedListener implements MouseListener {

	private AntiTD gameInstance;

	/**
	 * Saves the reference locally to the game instance controller
	 *
	 * @param gameInstance the controller
	 */
	public TilePressedListener(AntiTD gameInstance) {
		this.gameInstance = gameInstance;
	}

	@Override
	public void mouseClicked(MouseEvent e) {
	}

	/**
	 * Should run on buttonClick. It gets a scale from 0-1 corresponding where
	 * the user clicked the screen relative to the window size. It then
	 * calculates which tile were clicked and checks if it is a startPosition.
	 * If the clicked tile is a startPosition it sets the game states start
	 * position to the clicked tile. Else nothing happens.
	 * <p>
	 * Example: The user clicks square (0, 3) which is a start position in a
	 * 10x10 map. The x-scale is 0 and the y-scale is 0.35. xTile=0*10=0 and
	 * yTile=0.35*10=3.5 which is rounded down to an int of value 3. Square
	 * (0, 3) is now set as the start position.
	 */
	@Override
	public void mousePressed(MouseEvent e) {
		if (!gameInstance.isGamePaused()) {
			if (e.getButton() == MouseEvent.BUTTON1) {

				float percentageX = (float) e.getX() / (float) gameInstance.getWindow().getFrameWidth();
				float percentageY = (float) e.getY() / (float) gameInstance.getWindow().getFrameHeight();

				World world = gameInstance.getWorld();

				float x = percentageX * (float) world.getMapWidth();
				float y = percentageY * (float) world.getMapHeight();

				world.getCurrentLevel().getTile(x, y).onTileClicked(world);
			}
		}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}
}
