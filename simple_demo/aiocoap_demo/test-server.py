#!/usr/bin/env python3
import datetime
import logging
import asyncio

import aiocoap.resource as resource
import aiocoap

class TimeResource(resource.Resource):
    def __init__(self):
        super(TimeResource, self).__init__()

    async def render_get(self, request):
        payload = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S").encode('ascii')
        return aiocoap.Message(code=aiocoap.Code.CONTENT, payload=payload)

class BlockResource(resource.Resource):
    def __init__(self):
        super(BlockResource, self).__init__()
        self.content = ("123456789ABCDEF\n" * 16).encode("ascii")

    async def render_get(self, request):
        return aiocoap.Message(code=aiocoap.Code.CONTENT, payload=self.content)

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)

def main():
    root = resource.Site()
    root.add_resource(('.well-known', 'core'), resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(('time',), TimeResource())
    root.add_resource(('other', 'block'), BlockResource())
    asyncio.Task(aiocoap.Context.create_server_context(root))
    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()

