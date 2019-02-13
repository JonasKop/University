package controller;

import workers.ChannelUpdater;
import model.*;
import view.MainView;
import view.View;

import javax.swing.*;
import java.awt.event.ActionListener;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Controller in MVC architecture. It updates the channels and sets
 * the shows for the first ChannelTest and then schedules it to update every
 * hour. It also initializes the view and tells it to display when possible.
 */
public class Controller {

	/**
	 * Constructor which updates the radio channels and sets the shows for the
	 * first ChannelTest and then schedules it to rerun every hour. It also
	 * initializes the View and tells it to show when possible.
	 *
	 * @param model          the model that should be used
	 * @param view           the view that should be used
	 * @param updateChannels action listener which updates the channels
	 * @param updateShows    action listener which updates the shows
	 */
	private Controller(Model model, View view, ActionListener updateChannels,
					   ActionListener updateShows) {
		view.initialize(updateChannels, updateShows);
		SwingUtilities.invokeLater(view::display);

		ScheduledExecutorService ses =
				Executors.newSingleThreadScheduledExecutor();
		ses.scheduleAtFixedRate(() -> new ChannelUpdater(model, view).execute(),
				0, 1, TimeUnit.HOURS);
	}

	/**
	 * Main function which creates a model and a view and starts a controller.
	 *
	 * @param args not used
	 */
	public static void main(String[] args) {
		Model model = new MainModel();
		View view = new MainView();

		ActionListener act1 = (e) -> new ChannelUpdater(model, view).execute();
		ActionListener act2 = (e) -> model.clearSavedShows();
		new Controller(model, view, act1, act2);
	}
}
