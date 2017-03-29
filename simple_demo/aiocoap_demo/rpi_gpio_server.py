#!/usr/bin/env python3
import logging

import asyncio
import aiocoap.resource as resource
import aiocoap

import RPi.GPIO as GPIO
import json

led_pin = 11
class GPIOResource(resource.Resource):
    def __init__(self):
        super(GPIOResource, self).__init__()
        led_status = {'value': 0}
        self.content = json.dumps(led_status).encode("ascii")
    
    async def render_get(self, request):
        return aiocoap.Message(code=aiocoap.Code.CONTENT, payload=self.content)
  
    async def render_put(self, request):
        print('PUT payload: %s' % request.payload)
        led_status = json.loads(request.payload.decode())
        if led_status['value'] == 1 :
            print('open led')
            GPIO.output(led_pin, GPIO.HIGH)
        else :
            print('close led')
            GPIO.output(led_pin, GPIO.LOW)
        self.content = json.dumps(led_status).encode("ascii")
        return aiocoap.Message(code=aiocoap.Code.CHANGED, payload=self.content)

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)

def main():
    # setup gpio
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(led_pin, GPIO.OUT)

    # Resource tree creation
    root = resource.Site()
    root.add_resource(('.well-known', 'core'), resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(('gpio',), GPIOResource())
    asyncio.Task(aiocoap.Context.create_server_context(root))
    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()
