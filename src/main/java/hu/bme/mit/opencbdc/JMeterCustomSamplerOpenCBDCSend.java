package hu.bme.mit.opencbdc;

import org.apache.jmeter.config.Arguments;
import org.apache.jmeter.protocol.java.sampler.AbstractJavaSamplerClient;
import org.apache.jmeter.protocol.java.sampler.JavaSamplerContext;
import org.apache.jmeter.samplers.SampleResult;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
// import org.hyperledger.fabric.gateway.Contract;
// import org.hyperledger.fabric.gateway.Gateway;
// import org.hyperledger.fabric.gateway.Network;
// import org.hyperledger.fabric.gateway.FROM_Wallet;
/* 
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map; */

public class JMeterCustomSamplerOpenCBDCSend extends AbstractJavaSamplerClient {
    // private static volatile Map<String, Integer> nonces = new HashMap<String, Integer>();
    // private int retried = 0;
    // private Network network = null;
    private static final String FROM_MEMPOOL = "Mempool to transfer the funds from";
    private static final String FROM_WALLET = "Wallet to transfer the funds from";
    private static final String TO_ADDRESS = "Destination address";
    
    private static final Logger LOGGER = LoggerFactory.getLogger(JMeterCustomSamplerOpenCBDCSend.class);

    private String from_mempool;
    private String from_wallet;
    private String to_address;
    
    @Override
    public Arguments getDefaultParameters() {
        Arguments defaultParameters = new Arguments();
        defaultParameters.addArgument(FROM_MEMPOOL, null);
        defaultParameters.addArgument(FROM_WALLET, null);
        defaultParameters.addArgument(TO_ADDRESS, null);

        
        return defaultParameters;
    }
    public void initializeSampler(JavaSamplerContext javaSamplerContext){
        try {
         
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @Override
    public SampleResult runTest(JavaSamplerContext javaSamplerContext) {
         from_mempool = javaSamplerContext.getParameter(FROM_MEMPOOL);
         from_wallet = javaSamplerContext.getParameter(FROM_WALLET);
         to_address = javaSamplerContext.getParameter(TO_ADDRESS);

        OpenCBDCJavaClient client = new OpenCBDCJavaClient();

        SampleResult sampleResult = new SampleResult();
        sampleResult.sampleStart();
            sampleResult.setSampleLabel("Trans");

        try {
            String response = client.send(new String[]{from_mempool,from_wallet,to_address});
            sampleResult.sampleEnd();;
            sampleResult.setSuccessful(Boolean.TRUE);
            sampleResult.setResponseCodeOK();
            sampleResult.setResponseMessage(response);
            LOGGER.info("SUCCESS");
        } catch (Exception e) {
            LOGGER.error("Request was not successfully processed",e);
            sampleResult.sampleEnd();
            sampleResult.setResponseMessage(e.getMessage());
            sampleResult.setSuccessful(Boolean.FALSE);
        }
        return sampleResult;
    }
}