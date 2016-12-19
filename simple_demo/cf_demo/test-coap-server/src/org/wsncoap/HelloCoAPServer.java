package org.wsncoap;

import java.text.SimpleDateFormat; 
import java.util.Date;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapServer;
import org.eclipse.californium.core.coap.CoAP.ResponseCode;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class HelloCoAPServer {

    public static void main(String[] args) {
        
        CoapServer server = new CoapServer();
        server.add(new CoapResource("hello") {
            @Override
            public void handleGET(CoapExchange exchange) {
                exchange.respond(ResponseCode.CONTENT, "Hello CoAP!");
            }
        });
        server.add(new CoapResource("time") {
            @Override
            public void handleGET(CoapExchange exchange) {
                
                Date date = new Date();
                exchange.respond(ResponseCode.CONTENT, 
                            new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(date));
            }
        });
        server.start();
    }
}
