package view;

import java.awt.*;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Static class with private constructor which contains constants
 * about the style of the GUI.
 */
public final class MyStyle {

	public static final Color backgroundColor = new Color(0x212121);
	public static final Color accentColor = new Color(0x664d00);
	public static final Color passedColor = new Color(0x515151);
	public static final Color textColor = new Color(0xFAFAFA);
	public static final Color pressedBackgroundColor =
			new Color(255, 255, 255, 30);
	public static final String FONT_FAMILY = "Helvetica";

	private MyStyle() {
	}
}
