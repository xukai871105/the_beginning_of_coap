package org.wsncoap;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.cert.Certificate;
import java.util.logging.Level;
import java.util.Date;

import org.eclipse.californium.core.CaliforniumLogger;
import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapServer;
import org.eclipse.californium.core.coap.CoAP.ResponseCode;
import org.eclipse.californium.core.network.CoapEndpoint;
import org.eclipse.californium.core.network.Endpoint;
import org.eclipse.californium.core.network.config.NetworkConfig;
import org.eclipse.californium.core.network.interceptors.MessageTracer;
import org.eclipse.californium.core.server.resources.CoapExchange;
// import org.eclipse.californium.examples.SecureServer;
import org.eclipse.californium.scandium.DTLSConnector;
import org.eclipse.californium.scandium.ScandiumLogger;
import org.eclipse.californium.scandium.config.DtlsConnectorConfig;
import org.eclipse.californium.scandium.dtls.cipher.CipherSuite;
import org.eclipse.californium.scandium.dtls.pskstore.InMemoryPskStore;

public class SecureServer {
	
	static {
		CaliforniumLogger.initialize();
		CaliforniumLogger.setLevel(Level.CONFIG);
		ScandiumLogger.initialize();
		ScandiumLogger.setLevel(Level.FINER);
	}

	// allows configuration via Californium.properties
	public static final int DTLS_PORT = NetworkConfig.getStandard().getInt(NetworkConfig.Keys.COAP_SECURE_PORT);

	private static final String TRUST_STORE_PASSWORD = "rootPass";
	private final static String KEY_STORE_PASSWORD = "endPass";
	private static final String KEY_STORE_LOCATION = "certs/keyStore.jks";
	private static final String TRUST_STORE_LOCATION = "certs/trustStore.jks";
	
	public static void main(String[] args) {

		CoapServer server = new CoapServer();
		server.add(new CoapResource("secure") {
			@Override
			public void handleGET(CoapExchange exchange) {
				exchange.respond(ResponseCode.CONTENT, "Hello Security!");
			}
		});
		server.add(new CoapResource("time") {
			@Override
			public void handleGET(CoapExchange exchange) {
				Date date = new Date();
				exchange.respond(ResponseCode.CONTENT, date.toString());
			}
		});

		try {
			// Pre-shared secrets
			InMemoryPskStore pskStore = new InMemoryPskStore();
			pskStore.setKey("identity", "123456".getBytes()); 
			pskStore.setKey("pi", "raspberry".getBytes()); 

			// load the trust store
			KeyStore trustStore = KeyStore.getInstance("JKS");
			InputStream inTrust = SecureServer.class.getResourceAsStream(TRUST_STORE_LOCATION);
			trustStore.load(inTrust, TRUST_STORE_PASSWORD.toCharArray());

			// You can load multiple certificates if needed
			Certificate[] trustedCertificates = new Certificate[1];
			trustedCertificates[0] = trustStore.getCertificate("root");

			// load the key store
			KeyStore ks = KeyStore.getInstance("JKS");
			InputStream in = SecureServer.class.getClassLoader().getResourceAsStream(KEY_STORE_LOCATION);
			// InputStream in = SecureServer.class.getResourceAsStream(KEY_STORE_LOCATION);
			ks.load(in, KEY_STORE_PASSWORD.toCharArray());

			DtlsConnectorConfig.Builder config = new DtlsConnectorConfig.Builder(new InetSocketAddress(DTLS_PORT));
			config.setSupportedCipherSuites(new CipherSuite[]{CipherSuite.TLS_PSK_WITH_AES_128_CCM_8,
					CipherSuite.TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8});
			config.setPskStore(pskStore);
			config.setIdentity((PrivateKey)ks.getKey("server", KEY_STORE_PASSWORD.toCharArray()),
					ks.getCertificateChain("server"), true);
			config.setTrustStore(trustedCertificates);

			DTLSConnector connector = new DTLSConnector(config.build());

			server.addEndpoint(new CoapEndpoint(connector, NetworkConfig.getStandard()));
			server.start();

		} catch (GeneralSecurityException | IOException e) {
			System.err.println("Could not load the keystore");
			e.printStackTrace();
		}

		// add special interceptor for message traces
		for (Endpoint ep : server.getEndpoints()) {
			ep.addInterceptor(new MessageTracer());
		}

		System.out.println("Secure CoAP server is listening on port " + DTLS_PORT);
	}

}
