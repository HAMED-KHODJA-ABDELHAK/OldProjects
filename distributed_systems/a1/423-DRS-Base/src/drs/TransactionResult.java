package drs;

import java.io.Serializable;

/**
 * Simple class as part of the base of this system. Stringhe data will be whatever the
 * server needs to communicate back to the user.
 */
public class TransactionResult implements Serializable {
	private static final long serialVersionUID = 2L;
	private String data;
	private boolean success;

	public TransactionResult(String data, boolean success) {
		this.data = data;
		this.success = success;
	}

	public String getData() {
		return data;
	}

	public boolean suceeded() {
		return success;
	}

	@Override
	public String toString() {
		String text = "";

		if (success)
			text += String.format("Operation succeeded, returned: %s", data);
		else
			text += String.format("Operation failed, returned: %s", data);

		return text;
	}

	public static void main(String[] args) {
		TransactionResult result = new TransactionResult("Testing.", true);

		System.out.println(result);
	}
}
