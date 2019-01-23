/**
 * Created by dv16pen on 2017-05-08.
 */
public final class Config {
    private static final int chanceEventNum = 1;
    private static final int chanceEventDen = 10000;
    private static final int chanceAgentNum = 1;
    private static final int chanceAgentDen = 2;
    private static final int agentLife = 45;
    private static final int queryLife = 50;
    private static final int xMax = 50;
    private static final int yMax = 50;

    public static int getXMax() {
        return xMax;
    }

    public static int getYMax() {
        return yMax;
    }

    public static int getChanceEventNum() {
        return chanceEventNum;
    }

    public static int getChanceEventDen() {
        return chanceEventDen;
    }

    public static int getChanceAgentNum() {
        return chanceAgentNum;
    }

    public static int getChanceAgentDen() {
        return chanceAgentDen;
    }

    public static int getAgentLife() {
        return agentLife;
    }

    public static int getQueryLife() {
        return queryLife;
    }
}
