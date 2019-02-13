package model;

import channel.Channel;
import tools.Request;
import tools.XMLParser;

import java.io.IOException;
import java.util.ArrayList;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Model which contains channels and handles the usage of channels.
 */
public class MainModel implements Model {
	private static final String channelURL =
			"http://api.sr.se/api/v2/channels?&pagination=false";
	private ArrayList<Channel> channels;

	/**
	 * Gets the channels.
	 *
	 * @return the channels
	 */
	public synchronized ArrayList<Channel> getChannels() {
		return channels;
	}

	/**
	 * Updates the channels by sending a request and saving it to the show
	 * variable. If error occurs it sets the channels to null.
	 */
	public synchronized void updateChannels() {
		try {
			channels = XMLParser.parseChannels(Request.request(channelURL));
		} catch (IOException e) {
			e.printStackTrace();
			channels = null;
		}
	}

	/**
	 * Clears the saved shows in all channels by resetting them forcing update
	 * next time they want to be gotten.
	 */
	public synchronized void clearSavedShows() {
		for (Channel c : channels) {
			c.clearShows();
		}
	}
}
