package antitd.eventHandler;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;

import antitd.AntiTD;
import entities.EntityTroop;
import entities.effects.Effect;
import entities.troops.Troop;
import world.World;

/**
 * ActionListener specially made for the teleporter troop.
 *
 * @author Martin Hedberg, c17mhg
 */
public class TeleportActionListener implements ActionListener {

	private AntiTD gameInstance;

	/**
	 * Constructor which sets default values
	 *
	 * @param atd the current AntiTD instance
	 */
	public TeleportActionListener(AntiTD atd) {
		this.gameInstance = atd;
	}

	/**
	 * Spawns the set troop in the world
	 *
	 * @param e not used
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		new Thread(() -> {
			if (e.getSource() instanceof JButton) {
				JButton button = (JButton) e.getSource();

				World world = gameInstance.getWorld();
				EntityTroop troop = null;
				for (int i = 0; i < world.getTroops().size(); i++) {
					if (world.getTroops().get(i).getTroop() == Troop.teleportTroop) {
						troop = world.getTroops().get(i);
					}
				}

				if (troop == null) {
					gameInstance.getWorld().spawnTroop(Troop.teleportTroop);
					button.setText("Place teleport");
				} else {
					gameInstance.getWorld().setEffect(troop.getPos(), Effect.teleport);
					troop.kill();
					button.setText(Troop.teleportTroop.getName());
					button.setEnabled(false);
				}
			}
		}).start();

	}
}
