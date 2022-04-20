package hu.bme.mit.opencbdc;

public class OpenCBDCJavaClient {
	static {
		System.loadLibrary("opencbdc");
	}

	native boolean mint(String[] args);
	native boolean send(String[] args);
	native boolean fan(String[] args);
	native boolean sync(String[] args);
	native boolean newAddress(String[] args);
	native boolean info(String[] args);
	native boolean importInput(String[] args);
	native boolean confirmTx(String[] args);
}
