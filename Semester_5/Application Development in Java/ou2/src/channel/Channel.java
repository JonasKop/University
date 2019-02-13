package channel;

import tools.Request;
import tools.XMLParser;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.concurrent.locks.ReentrantLock;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Channel file containing name, id and icon of a ChannelTest. It also
 * contains the shows of the ChannelTest. All this can be gotten through getter
 * methods. The updateShows() method is slow since it sends a HTTP request and
 * parses a XML message, therefore it caches the saved shows and they are only
 * updated if clearShows is run.
 */
public class Channel {
	private String name;
	private int id;
	private BufferedImage icon;
	private ArrayList<RadioShow> shows = null;

	private static final String tableURL =
			"http://api.sr.se/api/v2/scheduledepisodes?channelid=";
	private String tableURLDate = "&date=";
	private static final String tableURLEnd = "&pagination=false";
	private String yesterdayURLEnd;
	private String todayURLEnd;
	private String tomorrowURLEnd;

	/**
	 * Constructor which saves variables,
	 *
	 * @param id   id of the ChannelTest
	 * @param name name of the ChannelTest
	 * @param icon icon of the ChannelTest
	 */
	public Channel(int id, String name, BufferedImage icon) {
		this.id = id;
		this.name = name;
		this.icon = icon;
	}

	/**
	 * Initializes the URLS making them possible to use for updating the shows.
	 */
	private void initURLs() {

		Date yesterday =
				new Date(System.currentTimeMillis() - 24 * 60 * 60 * 1000);
		Date today = new Date();
		Date tomorrow =
				new Date(System.currentTimeMillis() + 24 * 60 * 60 * 1000);

		yesterdayURLEnd = formatShowURLEnd(yesterday);
		todayURLEnd = formatShowURLEnd(today);
		tomorrowURLEnd = formatShowURLEnd(tomorrow);
	}

	/**
	 * Formats the show URL ending putting the date and ending in the end.
	 *
	 * @param day the day that should be formatted
	 * @return the string url
	 */
	private String formatShowURLEnd(Date day) {
		DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		return tableURLDate + dateFormat.format(day) + tableURLEnd;
	}

	/**
	 * Gets the name of the ChannelTest
	 *
	 * @return the name of the ChannelTest
	 */
	public String getName() {
		return name;
	}

	/**
	 * Gets the icon of the ChannelTest
	 *
	 * @return the icon of the ChannelTest
	 */
	public BufferedImage getIcon() {
		return icon;
	}

	/**
	 * Clears the shows by setting the to null.
	 */
	public synchronized void clearShows() {
		shows = null;
	}

	/**
	 * Updates the shows by initializing the url then it is sending a HTTP
	 * request and parsing the XML result and saves them to the shows variable.
	 */
	private synchronized void updateShows() {
		initURLs();
		try {
			String yesterday =
					Request.request(tableURL + id + yesterdayURLEnd);
			String today = Request.request(tableURL + id + todayURLEnd);
			String tomorrow =
					Request.request(tableURL + id + tomorrowURLEnd);

			ArrayList<RadioShow> yesterdaysShows =
					XMLParser.parseTable(yesterday);
			ArrayList<RadioShow> todaysShows = XMLParser.parseTable(today);
			ArrayList<RadioShow> tomorrowsShows =
					XMLParser.parseTable(tomorrow);

			assert yesterdaysShows != null;
			assert todaysShows != null;
			assert tomorrowsShows != null;

			yesterdaysShows.addAll(todaysShows);
			yesterdaysShows.addAll(tomorrowsShows);
			shows = yesterdaysShows;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Gets the shows. If they are null it updates and gets them. Else it just
	 * returns them.
	 *
	 * @return the shows
	 */
	public synchronized ArrayList<RadioShow> getShows() {
		if (shows == null) {
			updateShows();
		}
		return shows;
	}
}
