package hu.bme.mit.opencbdc;

public class OpenCBDCJavaClient {
	static {
		//System.loadLibrary("opencbdc");
	}

	String send(String[] args){
		String res = "";
		for (String string : args) {
			res+= string;	
			res += " ";
		}
		return res;
	}
	// native String mint(String[] args);
	// native String fan(String[] args);
	// native String sync(String[] args);
	// native String newAddress(String[] args);
	// native String info(String[] args);
	// native String importInput(String[] args);
	// native String confirmTx(String[] args);
}
