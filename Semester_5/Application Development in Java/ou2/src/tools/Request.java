package tools;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Static class which sends a HTTP request on an url and returns
 * the request result as a string.
 */
public final class Request {

	private Request() {
	}

	/**
	 * Sends a HTTP request to the given url and returns a string containing the
	 * result.
	 *
	 * @param url the url which should be requested
	 * @return string response from the HTTP server
	 * @throws IOException if it could not perform the request.
	 */
	public static String request(String url) throws IOException {
		URLConnection con = new URL(url).openConnection();
		InputStream stream = con.getInputStream();
		ByteArrayOutputStream buffer = new ByteArrayOutputStream();
		int nRead;
		byte[] data = new byte[16384];

		while ((nRead = stream.read(data, 0, data.length)) != -1) {
			buffer.write(data, 0, nRead);
		}

		return new String(buffer.toByteArray());
	}
}
