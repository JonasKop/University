package workers;

import channel.Channel;
import model.Model;
import channel.RadioShow;
import view.View;

import javax.swing.*;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: SwingWorker which updates the channels and publishes the changes
 * to the GUI which sets the view.
 */
public class ChannelUpdater extends SwingWorker<Boolean, Object[]> {
	private View view;
	private Model model;
	private Channel firstChannel = null;

	/**
	 * Constructor which saves the model and the view and if it should update
	 * the channels or just redraw them.
	 *
	 * @param model the model of the MVC
	 * @param view  the view of the MVC
	 */
	public ChannelUpdater(Model model, View view) {
		this.view = view;
		this.model = model;
		view.clearChannelPanel();
	}

	/**
	 * Updates the channels then it gets the channels an publishes
	 * them so they can be put on the view, then it sets the table so it
	 * matches the ChannelTest.
	 *
	 * @return if successful true, else false
	 */
	@Override
	protected Boolean doInBackground() {
		try {
			model.updateChannels();
		} catch (IOException e) {
			return false;
		}

		ArrayList<Channel> channels = model.getChannels();
		if (channels == null) {
			return false;
		}

		for (Channel c : channels) {
			if (firstChannel == null) {
				firstChannel = c;
			}
			ActionListener l = (e) -> new ShowGetter(view, c)
					.execute();
			Object[] obj = new Object[2];
			obj[0] = c;
			obj[1] = l;
			publish(obj);
		}
		return true;
	}

	/**
	 * Processes the published info. If it is a ChannelTest it adds it to the view.
	 * If it is a show it adds a show to the view.
	 *
	 * @param chunks list of Objects containing a show or a ChannelTest
	 */
	@Override
	protected void process(List<Object[]> chunks) {
		for (Object[] obj : chunks) {
			Channel c = (Channel) obj[0];
			view.addChannel(c.getName(), "Sveriges Radio", c.getIcon(),
					(ActionListener) obj[1]);

		}
		view.update();
	}

	/**
	 * Runs after doInBackground(). If failure it notifies the user of the
	 * problem, which in this case is a network problem.
	 */
	@Override
	protected void done() {

		try {
			if (!get()) {
				view.notifyUser("Could not connect to the servers. " +
						"Check your internet connection and try again.");
			} else {
				new ShowGetter(view, firstChannel).execute();
			}
		} catch (InterruptedException | ExecutionException e) {
			e.printStackTrace();
		}

	}
}