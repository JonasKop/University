package view;

import channel.RadioShow;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableModel;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: A view of an MVC architecture. It is the view of a radio tableau
 * program.
 */
public class MainView extends JFrame implements View {

	private JPanel channelPanel = new JPanel(), tablePanel = new
			JPanel(new BorderLayout()), descPanel = new JPanel(),
			contentPanel = new JPanel(new BorderLayout());
	private JLabel descLabel = new JLabel(), programTitle = new JLabel();
	private BufferedImage imgNotFound = null;

	/**
	 * Constructor which only runs super()
	 */
	public MainView() {
		super();
	}

	/**
	 * Initializes the view
	 *
	 * @param updateChannels action listener which updates the channels
	 * @param updateShows    action listener which updates the shows
	 */
	public void initialize(ActionListener updateChannels,
						   ActionListener updateShows) {
		getContentPane().setBackground(MyStyle.backgroundColor);
		channelPanel.setBackground(MyStyle.backgroundColor);
		descPanel.setBackground(MyStyle.backgroundColor);
		contentPanel.setBackground(MyStyle.backgroundColor);
		tablePanel.setBackground(MyStyle.backgroundColor);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setMinimumSize(new Dimension(1050, 650));
		setLocationRelativeTo(null);
		setJMenuBar(new MainMenuBar(updateChannels, updateShows));
		setTitle("RadioInfo");
		descPanel.setLayout(new BorderLayout());
		programTitle.setBorder(new EmptyBorder(10, 0, 0, 0));

		programTitle.setFont(new Font(MyStyle.FONT_FAMILY, Font.BOLD, 22));
		try {
			imgNotFound = ImageIO.read(getClass().getClassLoader()
					.getResourceAsStream("img_not_found.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}

		channelPanel.setLayout(new BoxLayout(channelPanel, BoxLayout.Y_AXIS));
		JScrollPane channelScrollPane = new JScrollPane(channelPanel,
				ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
				ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		channelScrollPane.setBackground(MyStyle.backgroundColor);
		add(channelScrollPane, BorderLayout.WEST);

		JScrollPane descScrollPane = new JScrollPane(tablePanel,
				ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
				ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		descScrollPane.setBackground(MyStyle.backgroundColor);

		contentPanel.add(descScrollPane, BorderLayout.CENTER);
		add(contentPanel, BorderLayout.CENTER);
	}

	/**
	 * Creates a table by given data and shows it on the view.
	 *
	 * @param data  the data that should be presented in the table
	 * @param shows the shows the data describes
	 */
	public void createTable(Object[][] data, ArrayList<RadioShow> shows) {
		tablePanel.removeAll();
		tablePanel.setBackground(MyStyle.backgroundColor);

		String[] columns = new String[]{"Program", "Starttime", "Endtime"};

		TableModel model = new DefaultTableModel(data, columns) {
			public boolean isCellEditable(int row, int column) {
				return false;
			}
		};

		JTable table = new JTable(model);
		table.setBackground(MyStyle.backgroundColor);
		table.setForeground(MyStyle.textColor);
		tablePanel.add(table, BorderLayout.CENTER);
		tablePanel.updateUI();

		table.setShowGrid(false);

		long currentTime = System.currentTimeMillis();

		int curr = 0;
		for (int i = 0; i < shows.size(); i++) {
			RadioShow s = shows.get(i);
			if (currentTime >= s.getStart().getTime() && currentTime <=
					s.getEnd().getTime()) {
				table.setRowSelectionInterval(i, i);
				String start = (String) data[i][1];
				String end = (String) data[i][2];
				contentPanel.add(updateDescPanel(shows.get(i), start, end),
						BorderLayout.SOUTH);
				curr = i;
				break;
			}
		}

		int finalCurr = curr;

		table.setDefaultRenderer(Object.class, new DefaultTableCellRenderer() {
			@Override
			public Component getTableCellRendererComponent(JTable table, Object
					value, boolean isSelected, boolean hasFocus, int row,
														   int column) {
				Component c = super.getTableCellRendererComponent(table, value,
						isSelected, hasFocus, row, column);
				c.setBackground(isSelected ? MyStyle.accentColor : row <
						finalCurr ? MyStyle.passedColor :
						MyStyle.backgroundColor);
				return c;
			}
		});

		table.getSelectionModel().addListSelectionListener(event -> {
			int sel = table.getSelectedRow();
			String start = (String) data[sel][1];
			String end = (String) data[sel][2];
			updateDescPanel(shows.get(sel), start, end);
		});
	}

	/**
	 * Updates a description panel with a show, start time and end time.
	 *
	 * @param show  the radio show
	 * @param start the start time
	 * @param end   the end time
	 * @return the updated description panel
	 */
	private JPanel updateDescPanel(RadioShow show, String start, String end) {
		descPanel.removeAll();
		descPanel.setBackground(MyStyle.backgroundColor);
		descPanel.add(createImage(show.getImg()), BorderLayout.WEST);
		descPanel.add(createDesc(show.getTitle(), show.getDesc()),
				BorderLayout.CENTER);
		JPanel pan = new JPanel();
		pan.setBorder(new EmptyBorder(5, 5, 0, 8));
		pan.setLayout(new BoxLayout(pan, BoxLayout.Y_AXIS));
		pan.setBackground(MyStyle.backgroundColor);

		JLabel startLabel = new JLabel("Start: " + start);
		startLabel.setForeground(MyStyle.textColor);
		JLabel endLabel = new JLabel("End:   " + end);
		endLabel.setForeground(MyStyle.textColor);
		pan.add(startLabel);
		pan.add(endLabel);

		descPanel.add(pan, BorderLayout.EAST);
		return descPanel;
	}

	/**
	 * Creates a description with a title and a description
	 *
	 * @param title the title of the show
	 * @param desc  the description of the show
	 * @return the description panel
	 */
	private JPanel createDesc(String title, String desc) {
		JPanel info = new JPanel(new BorderLayout());
		info.setBackground(MyStyle.backgroundColor);
		info.add(programTitle, BorderLayout.NORTH);
		info.add(descLabel, BorderLayout.CENTER);
		programTitle.setForeground(MyStyle.textColor);
		programTitle.setText(title);

		String htmlDesc = "<html><div WIDTH=%d>%s</div><html>";
		String formattedDesc = String.format(htmlDesc, 400, desc);

		descLabel.setForeground(MyStyle.textColor);
		descLabel.setText(formattedDesc);
		return info;
	}

	/**
	 * Creates an image and returns it of size 100x100 as a JLabel.
	 *
	 * @param programImage the image that should be put in a JLabel
	 * @return image as a JLabel.
	 */
	private JLabel createImage(BufferedImage programImage) {
		BufferedImage img = programImage == null ? imgNotFound : programImage;
		BufferedImage scaledImg = getScaledImage(img, 100, 100);
		JLabel picLabel = new JLabel(new ImageIcon(scaledImg));
		picLabel.setBorder(new EmptyBorder(10, 10, 10, 10));
		return picLabel;
	}

	/**
	 * Scales an image to the given width and height.
	 *
	 * @param img    the image that should be scaled
	 * @param width  the width of the new image
	 * @param height the height of the new image
	 * @return the scaled image
	 */
	private BufferedImage getScaledImage(Image img, int width, int height) {
		BufferedImage resizedImg = new BufferedImage(width, height,
				BufferedImage.TRANSLUCENT);
		Graphics2D g2 = resizedImg.createGraphics();
		g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
				RenderingHints.VALUE_INTERPOLATION_BILINEAR);
		g2.drawImage(img, 0, 0, width, height, null);
		g2.dispose();
		return resizedImg;
	}

	/**
	 * Updates the frame and repaints it
	 */
	public void update() {
		revalidate();
	}

	/**
	 * Notifies the user with a message.
	 *
	 * @param msg the message that should be shown to the user
	 */
	@Override
	public void notifyUser(String msg) {
		String htmlMsg = "<html><div WIDTH=%d>%s</div><html>";
		String formattedMsg = String.format(htmlMsg, 270, msg);
		JOptionPane.showMessageDialog(this, formattedMsg);
	}

	/**
	 * Clears the ChannelTest panel and update the UI
	 */
	public void clearChannelPanel() {
		channelPanel.removeAll();
		channelPanel.updateUI();
	}

	/**
	 * Adds a ChannelTest to the ChannelTest panel.
	 *
	 * @param channel  the channels name
	 * @param station  the stations name
	 * @param icon     the channels icon
	 * @param listener an actionlistener to the channelbutton
	 */
	public void addChannel(String channel, String station, BufferedImage icon,
						   ActionListener listener) {
		JPanel container = new JPanel();
		container.setBackground(MyStyle.backgroundColor);
		container.setBorder(new EmptyBorder(0, 0, -4, 0));
		ChannelButton channelButton = new ChannelButton(channel, station, icon);
		channelButton.addActionListener(listener);
		container.add(channelButton);
		channelPanel.add(container);
	}

	/**
	 * Displays the frame to the user.
	 */
	@Override
	public void display() {
		setVisible(true);
	}
}
