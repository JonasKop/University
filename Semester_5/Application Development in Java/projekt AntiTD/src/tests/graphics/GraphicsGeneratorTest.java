package tests.graphics;

import antitd.AntiTD;
import graphics.GraphicsGenerator;
import map.GameMap;
import map.MapException;
import map.ParseMap;
import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

/**
 * @author Petter Skogh (id16psh)
 * @version 1.0
 * Email: id16psh@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Description: Test for GraphicsGenerator. The test creates images from
 * GraphicsGenerator's methods so that you can see that it works as it should
 */
public class GraphicsGeneratorTest {
    public static void main(String[] args) {

        ArrayList<GameMap> maps;
        try {
            maps = ParseMap.parse("test.xml");
        } catch (MapException e) {
            e.printStackTrace();
            return;
        }

        GameMap levelMap = maps.get(1);

        /* Test: load image icon */
        BufferedImage bi = new BufferedImage(100*levelMap.getWidth(),
                100*levelMap.getHeight(), BufferedImage.TYPE_INT_ARGB);
        Image imageIcon = GraphicsGenerator.loadImageIcon("Banker").
                getImage();
        bi.createGraphics().drawImage(imageIcon, 0, 0, null);
        try {
            ImageIO.write(bi, "png", new File(
                    "src/tests/graphics/test_load_banker_icon.png"));
        } catch (IOException e) {
            e.printStackTrace();
        }

        /* Test: create the level image */
        try {
            BufferedImage img = GraphicsGenerator.createLevelImage(levelMap);
            ImageIO.write(img, "png", new File(
                    "src/tests/graphics/test_level.png"));
        } catch (IOException e) {
            e.printStackTrace();
        }

        AntiTD td = new AntiTD(maps);
        td.startNewGame(maps.get(1));
        /* Test: draw frame*/
        try {

            BufferedImage entitiesImg = GraphicsGenerator.drawFrame(td);
            ImageIO.write(entitiesImg, "png", new File(
                    "src/tests/graphics/test_entities.png"));
        } catch (IOException e) {
            e.printStackTrace();
        }

        BufferedImage combined = new BufferedImage(
                100*levelMap.getWidth(), 100*levelMap.getHeight(),
                BufferedImage.TYPE_INT_ARGB);
        /* Test: images look correct when combined */
        try {
            combined.createGraphics().drawImage(ImageIO.read(new File(
                    "src/tests/graphics/test_level.png")), 0, 0,
                    null);
            combined.createGraphics().drawImage(ImageIO.read(new File(
                    "src/tests/graphics/test_entities.png")), 0,
                    0, null);
            ImageIO.write(combined, "png", new File(
                    "src/tests/graphics/test_combined.png"));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
