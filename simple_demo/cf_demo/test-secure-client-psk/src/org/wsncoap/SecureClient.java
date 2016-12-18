package org.wsncoap;

import java.net.InetSocketAddress;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.logging.Level;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.Utils;
import org.eclipse.californium.core.network.CoapEndpoint;
import org.eclipse.californium.core.network.config.NetworkConfig;
import org.eclipse.californium.scandium.DTLSConnector;
import org.eclipse.californium.scandium.ScandiumLogger;
import org.eclipse.californium.scandium.config.DtlsConnectorConfig;
import org.eclipse.californium.scandium.dtls.pskstore.InMemoryPskStore;
import org.eclipse.californium.scandium.dtls.pskstore.StaticPskStore;

public class SecureClient {

	static {
		ScandiumLogger.initialize();
		ScandiumLogger.setLevel(Level.FINE);
	}

	private static final String SERVER_URI = "coaps://192.168.0.6/time";

	private DTLSConnector dtlsConnector;

	public SecureClient() {
		DtlsConnectorConfig.Builder builder = new DtlsConnectorConfig.Builder(new InetSocketAddress(0));
		
		InMemoryPskStore pskStore = new InMemoryPskStore();
		pskStore.setKey("identity", "123456".getBytes());
		builder.setPskStore(pskStore);
		builder.setPskStore(new StaticPskStore("pi", "raspberry".getBytes()));

		dtlsConnector = new DTLSConnector(builder.build());
	}

	public void test() {

		CoapResponse response = null;
		try {
			URI uri = new URI(SERVER_URI);

			CoapClient client = new CoapClient(uri);
			client.setEndpoint(new CoapEndpoint(dtlsConnector, NetworkConfig.getStandard()));
			response = client.get();

		} catch (URISyntaxException e) {
			System.err.println("Invalid URI: " + e.getMessage());
			System.exit(-1);
		}

		if (response != null) {

			System.out.println(response.getCode());
			System.out.println(response.getOptions());
			System.out.println(response.getResponseText());

			System.out.println("\nADVANCED\n");
			System.out.println(Utils.prettyPrint(response));

		} else {
			System.out.println("No response received.");
		}
	}

	public static void main(String[] args) throws InterruptedException {

		SecureClient client = new SecureClient();
		client.test();

		synchronized (SecureClient.class) {
			SecureClient.class.wait();
		}
	}
}
