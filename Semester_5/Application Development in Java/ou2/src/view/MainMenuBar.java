package view;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: MenuBar class containing two menu items with keyboard shortcuts.
 * One that updates the channels and one that updates the shows.
 */
class MainMenuBar extends JMenuBar {

	/**
	 * Constructor which creates two menu items with keyboard shortcuts. One
	 * that updates the channels and one that updates the shows. To update
	 * channels CMD/CTRL + C can be pressed and to update shows CMD/CTRL + S can
	 * be pressed.
	 *
	 * @param updateChannels action listener which updates the channels
	 * @param updateShows    action listener which updates the shows
	 */
	MainMenuBar(ActionListener updateChannels, ActionListener updateShows) {
		super();
		JMenu menu = new JMenu("Menu");
		add(menu);

		JMenuItem menuItem = new JMenuItem("Update Channels", KeyEvent.VK_T);
		menuItem.addActionListener(updateChannels);
		menuItem.setAccelerator(KeyStroke.getKeyStroke('C', Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		menu.add(menuItem);

		menuItem = new JMenuItem("Update Shows", KeyEvent.VK_T);
		menuItem.addActionListener(updateShows);
		menuItem.setAccelerator(KeyStroke.getKeyStroke('S', Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		menu.add(menuItem);
	}
}
