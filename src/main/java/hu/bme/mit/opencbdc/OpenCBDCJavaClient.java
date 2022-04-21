package hu.bme.mit.opencbdc;

public class OpenCBDCJavaClient {
	static {
		System.load("/opt/tx-processor/build/src/uhs/client/JmeterCustomSampler/libclient-jni.jnilib");
	}

	native String send(String[] args);
	native String mint(String[] args);
	native String fan(String[] args);
	native String sync(String[] args);
	native String newAddress(String[] args);
	native String info(String[] args);
	native String importInput(String[] args);
	native String confirmTx(String[] args);
}
