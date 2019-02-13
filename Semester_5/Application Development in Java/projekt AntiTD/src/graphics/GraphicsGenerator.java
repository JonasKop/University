package graphics;

import entities.*;
import entities.effects.Effect;
import map.*;
import primitives.Position;
import primitives.GroundType;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import antitd.AntiTD;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.List;

/**
 * @author Petter Skogh (id16psh), Martin Hedberg (c17mhg)
 * @version 1.0
 * Email: id16psh@cs.umu.se, c17mhg@cs.umu.se
 * Date: December 2018
 * <p>
 * Description: Generates graphics. Generates level and frame as BufferedImage.
 */
public class GraphicsGenerator {

	private static final int scale = 100;
	private static final String path = "images/";

	private static HashMap<String, BufferedImage> images = new HashMap<>();

	/**
	 * Creates level image
	 *
	 * @param levelMap - The map
	 * @return Image of map
	 */
	public static BufferedImage createLevelImage(GameMap levelMap) {


		BufferedImage levelImg = new BufferedImage(
				scale * levelMap.getWidth(),
				scale * levelMap.getHeight(),
				BufferedImage.TYPE_INT_ARGB);

		/* Loops through all squares in map and draws the ground type for
		 * each square
		 */
		for (int i = 0; i < levelMap.getWidth(); i++) {
			for (int j = 0; j < levelMap.getHeight(); j++) {
				Tile square = levelMap.getTile(i, j);
				GroundType squaresGroundType = square.getGround();
				switch (squaresGroundType) {
					case grass:
						levelImg.createGraphics().drawImage(
								loadOrGetImage("grass"), scale * i,
								scale * j, scale, scale, null);
						break;

					case desert:
						levelImg.createGraphics().drawImage(
								loadOrGetImage("desert"), scale * i,
								scale * j, scale, scale, null);
						break;
				}

				/* If square is road draws road over ground type */
				if (square.isRoad()) {
					levelImg.createGraphics().drawImage(loadOrGetImage("water"),
							scale * i, scale * j, scale, scale,
							null);
				}

				if (square.isStart()) {
					levelImg.createGraphics().drawImage(loadOrGetImage("Start"),
							scale * i, scale * j, scale, scale,
							null);
				}
				if (square.isGoal()) {
					levelImg.createGraphics().drawImage(loadOrGetImage("goal"),
							scale * i, scale * j, scale, scale,
							null);
				}
			}
		}
		return levelImg;
	}

	/**
	 * Draws everything that can be updated in the game.
	 * Draws:
	 * a start marker on the world's start position
	 * crossroad arrows and effects on road
	 * troops and towers
	 * HUD
	 *
	 * @param gameInstance - The instance of the game
	 * @return The frame as BufferedImage
	 */
	public static BufferedImage drawFrame(AntiTD gameInstance) {
		GameMap levelMap = gameInstance.getWorld().getCurrentLevel();
		List<EntityTroop> troops = gameInstance.getWorld().getTroops();
		List<EntityTower> towers = gameInstance.getWorld().getTowers();

		BufferedImage img = new BufferedImage(scale * levelMap.getWidth(),
				scale * levelMap.getHeight(),
				BufferedImage.TYPE_INT_ARGB);

		//Draw start marker
		Position start = gameInstance.getWorld().getStartPosition();
		img.createGraphics().drawImage(loadOrGetImage("start_marker"),
				(int) (start.getX() * scale) - scale / 2,
				(int) (start.getY() * scale) - scale / 2, scale, scale,
				null);

		//Draws crossroad arrows.
		//Draws effects
		for (Tile tile : gameInstance.getWorld().getRoad()) {
			int xPos = (int) (tile.getPosition().getX() * scale);
			int yPos = (int) (tile.getPosition().getY() * scale);

			Graphics2D g = img.createGraphics();
			double angle = Position.subtract(tile.getChoosenRoad(),
					tile.getPosition()).getAngle();
			g.rotate(-angle, xPos + scale / 2, yPos + scale / 2);

			if (tile.getRoad().size() > 1) {
				g.drawImage(loadOrGetImage("arrow"), xPos, yPos, scale, scale,
						null);
			}

			for (Effect e : tile.landOn()) {
				g.drawImage(loadOrGetImage(e.getName()), xPos, yPos, scale,
						scale, null);
			}
		}

		//Draws troops
		for (int i = 0; i < troops.size(); i++) {
			EntityTroop troop = troops.get(i);
			Position troopPos = troop.getPos();
			int xPos = Math.round(troopPos.getX() * scale);
			int yPos = Math.round(troopPos.getY() * scale);

			Graphics2D g = img.createGraphics();
			g.rotate(-troop.getRotation(), xPos, yPos);
			g.drawImage(loadOrGetImage(troop.getTroop().getImageName()),
					xPos - scale / 2, yPos - scale / 2, scale, scale,
					null);
		}

		//Draws turrets
		for (int i = 0; i < towers.size(); i++) {
			EntityTower tower = towers.get(i);
			Position towerPos = tower.getPos();
			int xPos = Math.round(towerPos.getX() * scale);
			int yPos = Math.round(towerPos.getY() * scale);

			Graphics2D g = img.createGraphics();
			g.rotate(-tower.getRotation(), xPos, yPos);
			g.drawImage(loadOrGetImage(tower.getTower().getImageName()),
					xPos - scale / 4, yPos - scale / 4, scale / 2,
					scale / 2, null);
			Position target = tower.getTargetPosition();
			if (target != null && tower.isFiring()) {
				img.createGraphics().drawLine((int) (target.getX() * scale),
						(int) (target.getY() * scale), xPos, yPos);
			}
		}

		return img;
	}

	/**
	 * Loads an image from the given file name as BufferedImage.
	 *
	 * @param image The name of the image file
	 * @return A BufferedImage of the image
	 */
	private static BufferedImage loadOrGetImage(String image) {
		if (!images.containsKey(image)) {
			URL url = GraphicsGenerator.class.getResource(path + image + ".png");
			if (url != null) {
				try {
					images.put(image, ImageIO.read(url));
				} catch (IOException e) {
					e.printStackTrace();
				}
			} else {
				try {
					images.put(image, ImageIO.read(GraphicsGenerator.class.
							getResource(path + "missingTexture.png")));
				} catch (IOException e1) {
					e1.printStackTrace();
				}
			}
		}

		return images.get(image);
	}

	/**
	 * Loads given file name as ImageIcon
	 *
	 * @param file - the file to be loaded
	 * @return An ImageIcon with the image
	 */
	public static ImageIcon loadImageIcon(String file) {
		URL url = GraphicsGenerator.class.getResource(path + file + ".png");
		if (url != null) {
			return new ImageIcon(url);
		}
		return new ImageIcon(GraphicsGenerator.class.getResource(path + "missingTexture.png"));
	}
}
