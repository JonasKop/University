public class Main {
	private static final int MAXTIME = 1000;
	private static final int xMax = 50;
	private static final int yMax = 50;

	public static void main(String[] args) {

		Time time = new Time();
		Network network = new Network(yMax, xMax);
		long timecount = System.nanoTime();

		while(time.getTime() < MAXTIME){
			network.createRandomEvent(time.getTime());
			network.messageUpdate(time.getTime());



			time.increment();
		}
		System.out.println((System.nanoTime()-timecount)/1000000000f);




	}
}
