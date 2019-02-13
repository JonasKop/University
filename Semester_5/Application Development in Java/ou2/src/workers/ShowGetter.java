package workers;

import channel.Channel;
import channel.RadioShow;
import view.View;

import javax.swing.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: SwingWorker class which gets a show and publishes it to the
 * view.
 */
public class ShowGetter extends SwingWorker<Boolean, Object[]> {

	private View view;
	private Channel channel;

	/**
	 * Constructor which saves variable locally.
	 *
	 * @param view    the MVC view
	 * @param channel ChannelTest which shows should be gotten.
	 */
	public ShowGetter(View view, Channel channel) {
		this.view = view;
		this.channel = channel;
	}

	/**
	 * Formats radio shows data so it can be fitted in a JTable.
	 *
	 * @param shows the radio shows that should be formatted
	 * @return the formatted data
	 */
	public static Object[][] formatData(ArrayList<RadioShow> shows) {
		Object[][] data = new Object[shows.size()][3];
		for (int i = 0; i < shows.size(); i++) {
			RadioShow show = shows.get(i);

			SimpleDateFormat formatter = new SimpleDateFormat("EEE HH:mm");
			data[i][0] = show.getTitle();
			data[i][1] = formatter.format(show.getStart());
			data[i][2] = formatter.format(show.getEnd());
		}
		return data;
	}

	/**
	 * Gets the shows from a ChannelTest and formats the show as data so it can be
	 * put into a JTable.
	 *
	 * @return if successful true, else false
	 */
	@Override
	protected Boolean doInBackground() {
		ArrayList<RadioShow> shows = channel.getShows();
		if (shows == null) {
			return false;
		}

		Object[][] obj = formatData(shows);
		Object[] ret = new Object[2];
		ret[0] = obj;
		ret[1] = shows;

		publish(ret);
		return true;
	}

	/**
	 * Publishes the radio show JTable data to the view.
	 *
	 * @param chunks the radio show JTable data.
	 */
	@Override
	protected void process(List<Object[]> chunks) {
		Object[] obj = chunks.get(0);
		Object[][] table = (Object[][]) obj[0];

		@SuppressWarnings("unchecked")
		ArrayList<RadioShow> shows = (ArrayList<RadioShow>) obj[1];
		view.createTable(table, shows);
	}

	/**
	 * Runs after doInBackground(). If failure it notifies the user of the
	 * problem, which in this case is a network problem.
	 */
	@Override
	protected void done() {
		try {
			if (!get()) {
				view.notifyUser("Could not connect to the servers. Check your internet connection and try again.");
			}
		} catch (InterruptedException | ExecutionException e) {
			e.printStackTrace();
		}
	}
}