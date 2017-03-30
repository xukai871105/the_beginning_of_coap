
#include <SPI.h>
#include <Ethernet.h>
#include <stdint.h>
#include <EthernetUdp.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "coap.h"

#define PORT 5683
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress ip(192, 168, 0, 10);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetClient client;
EthernetUDP udp;

#define WS2812_PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, WS2812_PIN, NEO_GRB + NEO_KHZ800);

uint8_t packetbuf[256];
static uint8_t scratch_raw[32];
static coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};

static int red = 10;
static int green = 10;
static int blue = 10;

static const coap_endpoint_path_t path_hello = {1, {"hello"}};
static int handle_get_hello(coap_rw_buffer_t *scratch, 
                            const coap_packet_t *inpkt, coap_packet_t *outpkt, 
                            uint8_t id_hi, uint8_t id_lo)
{
  char hello[32] = "Hello CoAP!";
  return coap_make_response(scratch, outpkt, 
                          (const uint8_t *)&hello, strlen(hello), 
                          id_hi, id_lo, &inpkt->tok, 
                          COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static const coap_endpoint_path_t path_rgb = {1, {"rgb"}};
static int handle_get_rgb(coap_rw_buffer_t *scratch, 
                            const coap_packet_t *inpkt, coap_packet_t *outpkt, 
                            uint8_t id_hi, uint8_t id_lo)
{
  char response[64];
  int len;
  StaticJsonBuffer<64> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["red"] = red;
  root["green"] = green;
  root["blue"] = blue;

  memset(response, 0x00, sizeof(response));
  root.printTo(response, sizeof(response));

  return coap_make_response(scratch, outpkt, 
                          (const uint8_t *)&response, strlen(response), 
                          id_hi, id_lo, &inpkt->tok, 
                          COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_rgb(coap_rw_buffer_t *scratch, 
                          const coap_packet_t *inpkt, coap_packet_t *outpkt, 
                          uint8_t id_hi, uint8_t id_lo)
{
  char request[64];
  memset(request, 0x00, sizeof(request));
  memcpy(request, inpkt->payload.p, inpkt->payload.len);

  StaticJsonBuffer<64> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(request);

  red = root["red"];
  green = root["green"];
  blue= root["blue"];

  color_wipe(strip.Color(red, green, blue));

  return coap_make_response(scratch, outpkt, 
                            NULL, 0, 
                            id_hi, id_lo, &inpkt->tok, 
                            COAP_RSPCODE_CHANGED, 
                            COAP_CONTENTTYPE_TEXT_PLAIN);
}

coap_endpoint_t endpoints[] =
{
  {COAP_METHOD_GET, handle_get_hello, &path_hello, "ct=0"},
  {COAP_METHOD_GET, handle_get_rgb, &path_rgb, "ct=40"},
  {COAP_METHOD_PUT, handle_put_rgb, &path_rgb, NULL},
  {(coap_method_t)0, NULL, NULL, NULL}
};

void endpoint_setup(void)
{                

}

void color_wipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void udp_send(const uint8_t *buf, int buflen)
{
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  while(buflen--)
      udp.write(*buf++);
  udp.endPacket();
}

void setup()
{
  int i;
  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  Serial.print("My IP address: ");
  for (i = 0; i < 4; i++)
  {
      Serial.print(Ethernet.localIP()[i], DEC);
      Serial.print("."); 
  }
  Serial.println();
  udp.begin(PORT);

  strip.begin();
  strip.show(); 

  coap_setup();
  endpoint_setup();
}

void loop()
{
    int sz;
    int rc;
    coap_packet_t pkt;
    int i;
    
    if ((sz = udp.parsePacket()) > 0)
    {
        udp.read(packetbuf, sizeof(packetbuf));

        Serial.print("UDP:");
        for (i = 0; i < sz; i++)
        {
            Serial.print(packetbuf[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");

        if (0 != (rc = coap_parse(&pkt, packetbuf, sz)))
        {
            Serial.print("Bad packet rc=");
            Serial.println(rc, DEC);
        }
        else
        {
            size_t rsplen = sizeof(packetbuf);
            coap_packet_t rsppkt;
            coap_handle_req(&scratch_buf, &pkt, &rsppkt, endpoints);

            memset(packetbuf, 0, UDP_TX_PACKET_MAX_SIZE);
            if (0 != (rc = coap_build(packetbuf, &rsplen, &rsppkt)))
            {
                Serial.print("coap_build failed rc=");
                Serial.println(rc, DEC);
            }
            else
            {
                udp_send(packetbuf, rsplen);
            }
        }
    }
}

