package view;

import view.MyStyle;

import java.awt.*;
import java.awt.image.BufferedImage;
import javax.swing.*;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Custom JButton which contains a Image, a ChannelTest and a station.
 * It has a fixed size and its size should ONLY be change within this class and
 * not by other methods. It uses styling from MyStyle constant file.
 */
public class ChannelButton extends JButton {
	private String channel, station;
	private BufferedImage icon;
	private final int WIDTH = 330, HEIGHT = 80;

	/**
	 * Constructor which saves variables
	 *
	 * @param channel the ChannelTest name
	 * @param station the station name
	 * @param icon    the icon
	 */
	ChannelButton(String channel, String station, BufferedImage icon) {
		super();
		this.channel = channel;
		this.station = station;
		this.icon = icon;
	}

	/**
	 * Makes the JButton always have the same size.
	 *
	 * @return se getPreferredSize()
	 */
	@Override
	public Dimension getMaximumSize() {
		return getPreferredSize();
	}

	/**
	 * Paints the JButton. It first draws a background color then the image to
	 * the left, then ChannelTest name and station name. If clicked it draws a semi
	 * transparent rectangle over the button showing that the button is clicked.
	 *
	 * @param g the graphics of the JButton
	 */
	@Override
	protected void paintComponent(Graphics g) {
		//Draw background
		g.setColor(MyStyle.backgroundColor);
		g.fillRect(0, 0, getWidth(), getHeight());

		g.drawImage(icon, 0, 0, HEIGHT, HEIGHT, null);

		g.setColor(MyStyle.textColor);
		g.setFont(new Font(MyStyle.FONT_FAMILY, Font.BOLD, 18));
		g.drawString(channel, 90, 35);
		g.setFont(new Font(MyStyle.FONT_FAMILY, Font.PLAIN, 14));
		g.drawString(station, 90, 60);

		if (getModel().isPressed()) {
			g.setColor(MyStyle.pressedBackgroundColor);
			g.fillRect(0, 0, getWidth(), getHeight());
		}
	}

	/**
	 * Sets constant width and height of the JButton.
	 *
	 * @return dimension of width and height.
	 */
	@Override
	public Dimension getPreferredSize() {
		return new Dimension(WIDTH, HEIGHT);
	}
}