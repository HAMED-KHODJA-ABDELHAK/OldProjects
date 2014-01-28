package common;

import java.io.IOException;

public class InvManager {
	private final static String[] servers = {"M", "T", "V"};
	private static String STORE_SEP = "/";
	private static String VAL_SEP = "|";

	private String message;

	public InvManager() {
	}

	public void getLocalInventory() throws IOException {
		message = "";

		for (String s : servers) {
			InvFile i = new InvFile(s);
			String state = s + VAL_SEP + i.getState() + STORE_SEP;
			message += state;
		}
	}

	@Override
	public String toString() {
		return message;
	}

	public void updateLocalInventory(String payload) throws IOException {
		String[] stores = payload.split(STORE_SEP);

		for (String store : stores) {
			String[] split = store.split(VAL_SEP);
			String storeName = split[0];
			InvFile i = new InvFile(storeName);
			i.writeState(split[0]);
		}
	}
}
