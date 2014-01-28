package common;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.StringTokenizer;

public class InvFile {
	private File file;
	private boolean opened;

	private final static String INV_SUFFIX = "-Inv";
	private final static String SEP_VAL = "_";

	public InvFile(String store) {
		this.opened = false;
		file = new File(store + INV_SUFFIX);

	}

	private void openFile() throws IOException {
		if  (opened == false) {

			if (!file.exists())
				file.createNewFile();

			opened = true;
		}
	}

	public synchronized void writeState(Map<String, Integer> inventory) throws IOException {
		openFile();

		StringBuilder s = new StringBuilder();

		for (String key : inventory.keySet()) {
			s.append(key);
			s.append(SEP_VAL);
			s.append(inventory.get(key));
			s.append(SEP_VAL);
		}

		BufferedWriter out = new BufferedWriter(new FileWriter(file));
		out.write((s.toString() + "\n"));
		out.close();
	}

	public void writeState(String state) throws IOException {
		openFile();

		BufferedWriter out = new BufferedWriter(new FileWriter(file));
		out.write(state + "\n");
		out.close();
	}

	public String getState() throws IOException {
		openFile();

		Scanner s = new Scanner(new FileInputStream(file));

		if (s.hasNextLine())
			return s.nextLine();
		else
			return "";
	}

	public Map<String, Integer> getMappedState() throws IOException {
		openFile();

		String s = getState();
		HashMap<String, Integer> inventory = new HashMap<String, Integer>();

		StringTokenizer tok = new StringTokenizer(s, SEP_VAL);

		while (tok.hasMoreTokens()) {
			String key = tok.nextToken();
			Integer val = (int) Double.parseDouble(tok.nextToken());
			inventory.put(key, val);
		}

		return inventory;
	}

	public static void main(String[] args) throws IOException {
		InvFile f = new InvFile("M");
		f.openFile();
		HashMap<String, Integer> map = new HashMap<String, Integer>();
		map.put("222", 44);
		map.put("244", 33);
		f.writeState(map);
		System.out.println(f.getState());

		Map<String, Integer> c = f.getMappedState();

		for (String s: c.keySet()) {
			System.out.println(String.format("%s_%s", s, c.get(s)));
		}

		System.out.println("Done.");
	}
}
