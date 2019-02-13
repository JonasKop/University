package model;

import channel.Channel;

import java.io.IOException;
import java.util.ArrayList;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Model interface for MVC Architecture. It handles the usage of
 * channels.
 */
public interface Model {

	/**
	 * Gets the channels.
	 *
	 * @return the channels
	 */
	ArrayList<Channel> getChannels();

	/**
	 * Updates the channels.
	 *
	 * @throws IOException if it could not update the channels
	 */
	void updateChannels() throws IOException;

	/**
	 * Clears the saved shows of the channels.
	 */
	void clearSavedShows();
}
