package tools;

import channel.Channel;
import channel.RadioShow;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import javax.imageio.ImageIO;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: January 2018
 * <p>
 * Description: Static class which parses XML. It can parse SR channels XML and
 * SR shows XML taken as a single string. These methods can only be accessed
 * statically.
 */
public final class XMLParser {

	private XMLParser() {
	}

	/**
	 * Parses the SR channels and saves their id, name and icon to a ChannelTest
	 * ArrayList which it then returns. If error it returns null
	 *
	 * @param channelXML xml string containing the SR channels.
	 * @return ArrayList containing channels if successful, else null
	 */
	public static ArrayList<Channel> parseChannels(String channelXML) {
		ArrayList<Channel> channels = new ArrayList<>();
		try {
			Document document = convertStringToDocument(channelXML);
			NodeList channelList = document.getElementsByTagName("channel");

			for (int i = 0; i < channelList.getLength(); i++) {
				Node n = channelList.item(i);


				int id = Integer.parseInt(n.getAttributes().getNamedItem("id")
						.getNodeValue());
				String name = n.getAttributes().getNamedItem("name")
						.getNodeValue();

				Element elem = (Element) n;
				String imageURL = elem.getElementsByTagName("image").item(0)
						.getTextContent();
				BufferedImage icon = ImageIO.read(new URL(imageURL));
				channels.add(new Channel(id, name, icon));
			}
			return channels;
		} catch (IOException | ParserConfigurationException | SAXException e) {
			e.printStackTrace();
		}

		return null;
	}

	/**
	 * Parses the SR tables and saves their title, start time, end time,
	 * description and image. If successful it returns an ArrayList of the
	 * shows, on error it returns null.
	 *
	 * @param showsXML xml string containing the SR shows
	 * @return ArrayList containing shows if successful, else null
	 */
	public static ArrayList<RadioShow> parseTable(String showsXML) {

		ArrayList<RadioShow> table = new ArrayList<>();
		Date now = new Date(System.currentTimeMillis());

		long earlier = new Date(now.getTime() - 12 * 60 * 60 * 1000).getTime();
		long later = new Date(now.getTime() + 12 * 60 * 60 * 1000).getTime();

		try {
			Document document = convertStringToDocument(showsXML);
			NodeList schedules = document.getElementsByTagName("schedule");
			Element e = (Element) schedules.item(0);
			NodeList nl = e.getElementsByTagName("scheduledepisode");


			for (int i = 0; i < nl.getLength(); i++) {
				e = (Element) nl.item(i);

				String startTime = e.getElementsByTagName("starttimeutc")
						.item(0).getTextContent();
				String endTime = e.getElementsByTagName("endtimeutc")
						.item(0).getTextContent();
				SimpleDateFormat dateFormat = new SimpleDateFormat(
						"yyyy-MM-dd'T'HH:mm:ss'Z'", new Locale("sv", "SE"));

				dateFormat.setTimeZone(TimeZone.getTimeZone("UTC"));
				Date start = dateFormat.parse(startTime);
				Date end = dateFormat.parse(endTime);

				if (end.getTime() < earlier || start.getTime() > later) {
					continue;
				}

				String title = e.getElementsByTagName("title").item(0)
						.getTextContent();

				BufferedImage img = null;
				try {
					String imageURL = e.getElementsByTagName("imageurl")
							.item(0).getTextContent();
					img = ImageIO.read(new URL(imageURL));
				} catch (NullPointerException ignored) {
				}

				String desc = e.getElementsByTagName("description")
						.item(0).getTextContent();
				table.add(new RadioShow(title, start, end, desc, img));
			}
			return table;
		} catch (IOException | ParserConfigurationException | SAXException |
				ParseException e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * Converts a string containing xml to a Document. It turns off a lot of XML
	 * validating features to improve speed since the XML files are so large.
	 *
	 * @param xml the string containing the xml.
	 * @return a document version of the xml.
	 * @throws ParserConfigurationException if it couldn't config the parser
	 * @throws IOException                  if it couldn't read the string xml
	 * @throws SAXException                 if a SAX error occurs
	 */
	private static Document convertStringToDocument(String xml)
			throws ParserConfigurationException, IOException, SAXException {

		DocumentBuilderFactory fac = DocumentBuilderFactory.newInstance();
		fac.setNamespaceAware(false);
		fac.setValidating(false);
		fac.setFeature("http://xml.org/sax/features/namespaces", false);
		fac.setFeature("http://xml.org/sax/features/validation", false);
		fac.setFeature("http://apache.org/xml/features/nonvalidating/" +
				"load-dtd-grammar", false);
		fac.setFeature("http://apache.org/xml/features/nonvalidating/" +
				"load-external-dtd", false);
		DocumentBuilder builder = fac.newDocumentBuilder();

		return builder.parse(new InputSource(new StringReader(xml)));
	}
}
