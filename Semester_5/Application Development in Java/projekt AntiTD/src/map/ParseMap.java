package map;

import primitives.GroundType;
import primitives.Position;
import org.w3c.dom.*;
import org.xml.sax.SAXException;

import entities.effects.Effect;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: A class which reads an XML file for all containing maps and
 * saves them to its local gameMaps variable. It can the be gotten through
 * a getGameMaps method. If a map is invalid it throws a MapException.
 */
public final class ParseMap {

	private ParseMap() {
	}

	private static boolean validateXML(InputStream xml, InputStream xsd) {
		try {
			SchemaFactory factory = SchemaFactory
					.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
			Schema s = factory.newSchema(new StreamSource(xsd));
			Validator v = s.newValidator();
			v.validate(new StreamSource(xml));
			return true;

		} catch (SAXException | IOException e) {
			return false;
		}
	}

	public static ArrayList<GameMap> parse(String filename) throws MapException {
		ArrayList<GameMap> gameMaps = new ArrayList<>();


		try {
			InputStream inpXML = ParseMap.class.getResource("/" + filename).openStream();
			InputStream inpXSD = ParseMap.class.getResource("/" + "validate.xsd").openStream();
			if (!validateXML(inpXML, inpXSD)) {
				System.err.println("Invalid XML file.");
				throw new MapException("Could not load file: " + filename);
			}
			inpXML = ParseMap.class.getResource("/" + filename).openStream();
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(inpXML);

			doc.getDocumentElement().normalize();
			NodeList mapList = doc.getElementsByTagName("maps");

			Node mapNode = mapList.item(0);

			Element mapElement = (Element) mapNode;
			mapList = mapElement.getElementsByTagName("gameMap");
			for (int i = 0; i < mapList.getLength(); i++) {
				if (mapList.item(i).getNodeType() == Node.ELEMENT_NODE) {
					createMap((Element) mapList.item(i), gameMaps);
				}
			}
		} catch (ParserConfigurationException | SAXException | IOException e) {
			e.printStackTrace();
			throw new MapException("Could not load file: " + filename);
		}
		return gameMaps;
	}

	/**
	 * Creates a Map from a given XML-element and adds the created map to the
	 * local arraylist containing GameMaps.
	 *
	 * @param mapElement the map that should be created.
	 * @throws MapException if invalid attributes.
	 */
	private static void createMap(Element mapElement,
								  ArrayList<GameMap> gameMaps) throws MapException {

		String name = mapElement.getAttribute("name");
		int width = Integer.parseInt(mapElement.getAttribute("width"));
		int height = Integer.parseInt(mapElement.getAttribute("height"));
		if (width <= 0 || height <= 0)
			throw new MapException("Invalid size");
		GroundType standardType = GroundType.valueOf(mapElement.getAttribute("ground-type"));

		NodeList tileList = mapElement.getElementsByTagName("tile");
		GameMap gameMap = new GameMap(name, width, height);
		for (int j = 0; j < tileList.getLength(); j++) {
			insertTile(gameMap, tileList.item(j).getAttributes(), standardType);
		}
		fillMap(gameMap, standardType);
		gameMaps.add(gameMap);
		if (!isCorrectMap(gameMap)) {
			throw new MapException("Invalid gameMap");
		}
	}

	/**
	 * Checks if a read map is correct. It checks if it has a start and a goal.
	 * If it has a nama and a size. It the returns the value. If a value is
	 * incorrect it returns false
	 *
	 * @param gameMap the gamemap that should be tested.
	 * @return true if correct else false
	 */
	private static boolean isCorrectMap(GameMap gameMap) {
		ArrayList<Position> starts = gameMap.getStarts();
		ArrayList<Position> goals = gameMap.getGoals();

		boolean correctStart = starts.size() != 0;
		for (Position p : starts) {
			if (!gameMap.getTile(p.getX(), p.getY()).isRoad()) {
				return false;
			}
		}

		boolean correctGoal = goals.size() != 0;
		for (Position p : goals) {
			if (!gameMap.getTile(p.getX(), p.getY()).isRoad()) {
				return false;
			}
		}

		boolean correctName = gameMap.getName() != null;
		boolean correctSize = gameMap.getHeight() > 0 && gameMap.getWidth() > 0;
		return correctGoal && correctStart && correctName && correctSize;
	}

	/**
	 * Gets the direction of a road tile and sets them to the local arraylist
	 * containing Positions which is then returned.
	 *
	 * @param item the node containing the directions
	 * @param x    the x coordinate
	 * @param y    the y coordinate
	 * @return an arraylist containing Positions
	 */
	private static ArrayList<Position> getRoadDirection(Node item, int x, int y) {
		ArrayList<Position> directions = new ArrayList<>();
		if (item == null) {
			return null;
		}
		String[] parts = item.getNodeValue().split("\\|");
		for (String part : parts) {
			switch (part) {
				case "e":
					directions.add(new Position(x + 1, y));
					break;
				case "s":
					directions.add(new Position(x, y + 1));
					break;
				case "w":
					directions.add(new Position(x - 1, y));
					break;
				case "n":
					directions.add(new Position(x, y - 1));
					break;
			}
		}
		return directions;
	}

	/**
	 * Inserts a tile to a gamemap with given attributes and a standard
	 * GroundType. If the GroundType is not set at the tile it sets the
	 * GroundType to the standard type. After all attributes are set it creates
	 * the tile and sets it as a goal or a start if it is.
	 *
	 * @param gameMap the gameMap which the tile should be inserted into.
	 * @param attr    attributes of the tile.
	 * @param type    standard GroundType of the map.
	 */
	private static void insertTile(GameMap gameMap, NamedNodeMap attr, GroundType type) {
		int x = Integer.parseInt(attr.getNamedItem("x").getNodeValue());
		int y = Integer.parseInt(attr.getNamedItem("y").getNodeValue());

		Node groundNode = attr.getNamedItem("ground-type");
		if (groundNode != null) {
			type = GroundType.valueOf(groundNode.getNodeValue());
		}

		ArrayList<Position> directions = getRoadDirection(attr.getNamedItem("road-direction"), x, y);
		boolean start = false, goal = false;
		ArrayList<Effect> effects = new ArrayList<>();

		Node specialVal = attr.getNamedItem("special");
		if (specialVal != null) {
			String[] parts = specialVal.getNodeValue().split("\\|");
			for (String part : parts) {
				switch (part) {
					case "start":
						start = true;
						break;
					case "goal":
						goal = true;
						break;
					case "health":
						effects.add(Effect.health);
						break;
					case "speed":
						effects.add(Effect.speed);
						break;
					case "teleport":
						effects.add(Effect.teleport);
						break;
				}
			}
		}
		gameMap.addTile(new Tile(x, y, type, directions, start, goal, effects));
		if (start) {
			gameMap.addStart(new Position(x, y));
		} else if (goal) {
			gameMap.addGoal(new Position(x, y));
		}
	}

	/**
	 * Fills all empty map squares with a standard square.
	 *
	 * @param gameMap the gameMap which the square should be inserted into.
	 * @param type    standard GroundType of the map.
	 */
	private static void fillMap(GameMap gameMap, GroundType type) {
		for (int i = 0; i < gameMap.getWidth(); i++) {
			for (int j = 0; j < gameMap.getHeight(); j++) {
				gameMap.addTile(new Tile(i, j, type, null, false, false, new ArrayList<>()));
			}
		}
	}
}
