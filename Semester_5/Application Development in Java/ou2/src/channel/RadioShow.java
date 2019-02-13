package channel;

import java.awt.image.BufferedImage;
import java.util.Date;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: RadioShow class containing info about a radio show. It contains
 * the title of the show, the start time of the show, the end time of the show,
 * the icon of the show and the description of the show. This info can only be
 * set by the constructor but it can be gotten by getter methods.
 */
public class RadioShow {

	private String title;
	private Date start, end;
	private BufferedImage img;
	private String desc;

	/**
	 * Constructor which sets multiple parameters describing the show.
	 *
	 * @param title title of the show
	 * @param start start time of the show
	 * @param end   end time of the show
	 * @param desc  description of the show
	 * @param img   image of the show
	 */
	public RadioShow(String title, Date start, Date end, String desc, BufferedImage img) {
		this.title = title;
		this.start = start;
		this.end = end;
		this.desc = desc;
		this.img = img;
	}

	/**
	 * Gets the title of the show.
	 *
	 * @return title of the show
	 */
	public String getTitle() {
		return title;
	}

	/**
	 * Gets the start time of the show.
	 *
	 * @return start time of the show
	 */
	public Date getStart() {
		return start;
	}

	/**
	 * Gets the end time of the show.
	 *
	 * @return end time of the show
	 */
	public Date getEnd() {
		return end;
	}

	/**
	 * Gets the image of the show.
	 *
	 * @return image of the show
	 */
	public BufferedImage getImg() {
		return img;
	}

	/**
	 * Gets the description of the show.
	 *
	 * @return description of the show
	 */
	public String getDesc() {
		return desc;
	}
}
