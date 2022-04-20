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
    private static final String FROM_MEMPOOL = "mempool to transfer the funds from";
    private static final String FROM_WALLET = "wallet to transfer the funds from";
    private static final String TO_ADDRESS = "destination address";
    
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
            boolean response = client.send(new String[]{from_mempool,from_wallet,to_address});
            sampleResult.sampleEnd();;
            sampleResult.setSuccessful(Boolean.TRUE);
            sampleResult.setResponseCodeOK();
            sampleResult.setResponseMessage(Boolean.toString(response));
            LOGGER.info("SUCCESS");
        } catch (Exception e) {
            LOGGER.error("Request was not successfully processed",e);
            sampleResult.sampleEnd();
            sampleResult.setResponseMessage(e.getMessage());
            sampleResult.setSuccessful(Boolean.FALSE);
        }
        return sampleResult;
    }
    /* public Network createConnection(String identity, String FROM_walletPath, String connectionPath, String channel){
        try {
            Path FROM_walletDirectory = Paths.get(FROM_walletPath);
            Path networkConfigFile = Paths.get(connectionPath);
            FROM_Wallet FROM_wallet = FROM_Wallet.createFileSystemFROM_Wallet(FROM_walletDirectory);
            Gateway.Builder builder = Gateway.createBuilder()
                    .identity(FROM_wallet, identity)
                    .networkConfig(networkConfigFile);
            return builder.connect().getNetwork(channel);
        }
        catch(Exception e){
            e.printStackTrace();
            return null;
        }
    } */
    /* public SampleResult createTransaction(Network network, Signature signature, String chaincode, String method, String submit, KeyPair signingKey, String... args) {
        SampleResult sampleResult = new SampleResult();
        try {
            ArrayList<String> argParts = new ArrayList(Arrays.asList(args));
            Contract contract = network.getContract(chaincode);
            byte[] result = null;
            if (signature != null) {
                argParts.add(String.valueOf(signature.v));
                argParts.add(signature.r);
                argParts.add(signature.s);
            }
            sampleResult.sampleStart();
            if (submit.equals("evaluate")){
                result = contract.createTransaction(method).evaluate(Arrays.copyOf(argParts.toArray(), argParts.size(), String[].class));
            }else{
                result = contract.createTransaction(method).submit(Arrays.copyOf(argParts.toArray(), argParts.size(), String[].class));
            }
            //serializeObject(noncesFile, nonces);
            sampleResult.sampleEnd();
            sampleResult.setSuccessful(Boolean.TRUE);
            sampleResult.setResponseCodeOK();
            sampleResult.setResponseMessage(new String(result, StandardCharsets.UTF_8));
            return sampleResult;
        }
        catch(Exception e){
            if(e.getMessage().contains("nonce")){
                e.printStackTrace();
                //Get good nonce
                System.out.println("OLD NONCE of "+ addressString + ": " + args[3]);
                Integer nonce = getNonceOnException(args[0])+1;
                System.out.println("NEW NONCE of "+ addressString + ": " + nonce);
                nonces.put(signingKey.addressString, nonce);
                args[3] = String.valueOf(nonce);
                //Sign tx
                signature = SignHomeNativeMessage.createSignatureFromKeyPair(signingKey, args);
                return createTransaction(network, signature, chaincode, method, submit, signingKey, args);
            }
            if (retried <= 1) {
                retried++;
                //Sleep
                try {Thread.sleep(10);} catch (InterruptedException ex) {ex.printStackTrace();}
                return createTransaction(network, signature, chaincode, method, submit, signingKey, args);
            }
            e.printStackTrace();
            sampleResult.setSuccessful(Boolean.FALSE);
            sampleResult.setResponseMessage(e.getMessage());
            return sampleResult;
        }
    } */
    /* public int getNonceOnException(String address) {
        try{
            return Integer.parseInt(createTransaction(network, null, chaincode, "getNonce","evaluate", null, address).getResponseMessage());
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return -1;
    } */
}