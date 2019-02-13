package database;

import java.sql.*;
import java.util.ArrayList;

/**
 * @author Jonas Sjödin (id16jsn)
 * @version 1.0
 * Email: jonas@jonassjodin.com
 * Date: December 2018
 * <p>
 * Description: Database connection which connects to a set domain with set
 * options. It can then retrieve the data from the table with getHighScore.
 * It can also insert a score into the highscore table. This class contains
 * only static methods and static final variables and a private constructor
 * meaning that it cannot be instantiated and should only be accessed
 * statically.
 */
public final class Database {
	private static final String hostname = "mysql.cs.umu.se", port = "3306", databaseName = "v135h18g5", table = "highscores", username = "v135h18g5", password = "CuPh1AtR1HXNtipV";
	private static final String url = "jdbc:mysql://" + hostname + ":" + port + "/" + databaseName + "?autoReconnect=true&useSSL=false";

	private Database() {
	}

	/**
	 * Gets the highscore from a set SQL database. Returns it as an arraylist
	 * containing scores which contains a name and a score.
	 *
	 * @param mapName the name of the map which scores should be received
	 * @return the arraylist containing the scores.
	 * @throws SQLException           if an SQL problem occurs
	 * @throws ClassNotFoundException if the com.mysql.jdbc.Driver is not
	 *                                installed
	 */
	public static ArrayList<Score> getHighScores(String mapName)
			throws SQLException, ClassNotFoundException {
		ArrayList<Score> scores = new ArrayList<>();

		Class.forName("com.mysql.jdbc.Driver");
		Connection con = DriverManager.getConnection(url, username, password);
		String query = "select * from " + table + " where mapname='" + mapName +
				"' order by score asc;";
		ResultSet rs = con.createStatement().executeQuery(query);

		while (rs.next()) {
			scores.add(new Score(rs.getString(1), rs.getInt(2)));
		}
		con.close();

		return scores;
	}

	/**
	 * Inserts a highscore into the SQL database. If an SQL problem occurs an
	 * SQLException is thrown. If the SQL drivers is not installed the a
	 * ClassNotFoundException is thrown.
	 *
	 * @param score   the score of the user containing its name and its score
	 * @param mapName the name of the map which scores should be received
	 * @throws SQLException           if an SQL problem occurs
	 * @throws ClassNotFoundException if the com.mysql.jdbc.Driver is not
	 *                                installed
	 */
	public static void insertHighScore(Score score, String mapName)
			throws SQLException, ClassNotFoundException {

		Class.forName("com.mysql.jdbc.Driver");
		Connection con = DriverManager.getConnection(url, username, password);
		String query = "INSERT INTO " + table + " VALUES('" + score.getName() +
				"', " + score.getScore() + ", '" + mapName + "')";
		con.createStatement().executeUpdate(query);
		con.close();
	}
}  