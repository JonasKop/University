package map;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: Exceptions which are thrown by the GameMap class.
 */
public class MapException extends Exception {
	/**
	 *
	 */
	private static final long serialVersionUID = 1L;

	public MapException() {
		super();
	}

	public MapException(String message) {
		super(message);
	}

	public MapException(String message, Throwable cause) {
		super(message, cause);
	}

	public MapException(Throwable cause) {
		super(cause);
	}
}

