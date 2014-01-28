package drs.client;

/**
 * Top class for the different kinds of clients.
 */
public abstract class ClientBase implements Runnable {
	protected String cID;
	private int delay;
	private Thread thread;

	public ClientBase(String cID, int delay) {
		this.cID = cID;
		this.delay = delay;
		thread = new Thread(this);
		thread.setName(cID);
	}

	/**
	 * Client simply examines the first character to know closes server.
	 * @return
	 */
	public char determineServerName() {
		return cID.toUpperCase().charAt(0);
	}

	public void start() {
		thread.start();
	}

	public void printWithID(String s) {
		System.out.println(cID + ":" + s);
	}

	/**
	 * Sleep for a given time, then execute a procedure that was overridden.
	 */
	@Override
	public void run() {
		try {
			Thread.sleep(delay);
			procedure();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public abstract void procedure();
}