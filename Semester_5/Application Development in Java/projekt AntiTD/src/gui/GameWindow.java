package gui;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * @author Marcus Jonsson (c17mjn)
 * @version 1.0
 * Email: c17mjn@cs.umu.se
 * Date: December 2018
 * <p>
 * Description: Game view of program.
 */
public class GameWindow extends JComponent {


	private static final long serialVersionUID = 1L;
	private BufferedImage activeFrame, backgroundImage;

	/**
	 * Graphical object containing 2 BufferedImages functioning as a background and active frame.
	 */
	public GameWindow() {

	}

	/**
	 * Method used to paint both BufferedImages.
	 */
	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(this.backgroundImage, 0, 0, this.getWidth(), this.getHeight(), null);
		g.drawImage(this.activeFrame, 0, 0, this.getWidth(), this.getHeight(), null);
	}

	/**
	 * Method used to set BufferedImage used as background image.
	 *
	 * @param background New BufferedImage to be used as background image.
	 */
	public void setBackground(BufferedImage background) {

		this.backgroundImage = background;
		this.repaint();
	}

	/**
	 * Method used to set BufferedImage used as active game frame.
	 *
	 * @param gameFrame New BufferedImage to be used as active game frame.
	 */
	public void updateFrame(BufferedImage gameFrame) {

		this.activeFrame = gameFrame;
		this.repaint();
	}
}