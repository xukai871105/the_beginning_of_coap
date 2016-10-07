
#include <SPI.h>
#include <Ethernet.h>
#include <stdint.h>
#include <EthernetUdp.h>
#include "coap.h"

#define PORT 5683
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress ip(192, 168, 0, 10);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetClient client;
EthernetUDP udp;
uint8_t packetbuf[256];
static uint8_t scratch_raw[32];
static coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};

static char light = '0';
static int led = 8;

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

static const coap_endpoint_path_t path_light = {1, {"light"}};
static int handle_get_light(coap_rw_buffer_t *scratch, 
                            const coap_packet_t *inpkt, coap_packet_t *outpkt, 
                            uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, 
                            (const uint8_t *)&light, 1, 
                            id_hi, id_lo, &inpkt->tok, 
                            COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_light(coap_rw_buffer_t *scratch, 
                            const coap_packet_t *inpkt, coap_packet_t *outpkt, 
                            uint8_t id_hi, uint8_t id_lo)
{
  if (inpkt->payload.p[0] == '1')
  {
      light = '1';
      digitalWrite(led, HIGH);
      Serial.println("ON");
  }
  else
  {
      light = '0';
      digitalWrite(led, LOW);
      Serial.println("OFF");
  }
  return coap_make_response(scratch, outpkt, 
                              (const uint8_t *)&light, 1, 
                              id_hi, id_lo, &inpkt->tok, 
                              COAP_RSPCODE_CHANGED, 
                              COAP_CONTENTTYPE_TEXT_PLAIN);
}

coap_endpoint_t endpoints[] =
{
  {COAP_METHOD_GET, handle_get_hello, &path_hello, "ct=0"},
  {COAP_METHOD_GET, handle_get_light, &path_light, "ct=0"},
  {COAP_METHOD_PUT, handle_put_light, &path_light, NULL},
  {(coap_method_t)0, NULL, NULL, NULL}
};

void endpoint_setup(void)
{                
    pinMode(led, OUTPUT);
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

    coap_setup();
    endpoint_setup();
}

void udp_send(const uint8_t *buf, int buflen)
{
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    while(buflen--)
        udp.write(*buf++);
    udp.endPacket();
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

