package view;

import channel.RadioShow;

import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.util.ArrayList;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: View interface for a radio ChannelTest tableau GUI.
 */
public interface View {

	/**
	 * Displays the window
	 */
	void display();

	/**
	 * Adds a channelButton to the view.
	 *
	 * @param channel  the channels name
	 * @param station  the stations name
	 * @param icon     the channels icon
	 * @param listener an actionlistener to the channelbutton
	 */
	void addChannel(String channel, String station, BufferedImage icon, ActionListener listener);

	/**
	 * Initializes the view.
	 *
	 * @param updateChannels action listener which updates the channels
	 * @param updateShows    action listener which updates the shows
	 */
	void initialize(ActionListener updateChannels, ActionListener updateShows);

	/**
	 * Clears the panel showing the channelButtons.
	 */
	void clearChannelPanel();

	/**
	 * Repaints the window.
	 */
	void update();

	/**
	 * Notifies the user with a message.
	 *
	 * @param msg the message that should be shown to the user
	 */
	void notifyUser(String msg);

	/**
	 * Creates a table with the given data.
	 *
	 * @param data  the data that should be presented in the table
	 * @param shows the shows the data describes
	 */
	void createTable(Object[][] data, ArrayList<RadioShow> shows);
}
