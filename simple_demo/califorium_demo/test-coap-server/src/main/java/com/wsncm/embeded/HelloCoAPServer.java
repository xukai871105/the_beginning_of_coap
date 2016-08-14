package com.wsncm.embeded;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketException;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapServer;
import org.eclipse.californium.core.network.CoapEndpoint;
import org.eclipse.californium.core.network.EndpointManager;
import org.eclipse.californium.core.network.config.NetworkConfig;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class HelloCoAPServer extends CoapServer {
    private static final int COAP_PORT = NetworkConfig.getStandard()
            .getInt(NetworkConfig.Keys.COAP_PORT);
    public static void main(String[] args) {
        try {
            // create server
            HelloCoAPServer server = new HelloCoAPServer();
            // add endpoints on all IP addresses
            server.addEndpoints();
            server.start();
        } catch (SocketException e) {
            System.err.println("Failed to initialize server: " + e.getMessage());
        }
    }

    private void addEndpoints() {
        for (InetAddress addr : EndpointManager.getEndpointManager().getNetworkInterfaces()) {
            // only binds to IPv4 addresses and localhost
            if (addr instanceof Inet4Address || addr.isLoopbackAddress()) {
                InetSocketAddress bindToAddress = new InetSocketAddress(addr, COAP_PORT);
                addEndpoint(new CoapEndpoint(bindToAddress));
            }
        }
    }

    public HelloCoAPServer() throws SocketException {

        // provide an instance of a Hello-World resource
        add(new HelloWorldResource());
    }

    class HelloWorldResource extends CoapResource {

        public HelloWorldResource() {
            // set resource identifier
            super("hello");
            // set display name
            getAttributes().setTitle("Hello-World Resource");
        }

        @Override
        public void handleGET(CoapExchange exchange) {
            // respond to the request
            exchange.respond("Hello CoAP!");
        }
    }
}
