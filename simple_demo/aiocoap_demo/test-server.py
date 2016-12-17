#!/usr/bin/env python3
import datetime
import logging

import asyncio

import aiocoap.resource as resource
import aiocoap

class TimeResource(resource.ObservableResource):
    def __init__(self):
        super(TimeResource, self).__init__()
        self.notify()

    def notify(self):
        self.updated_state()
        asyncio.get_event_loop().call_later(10, self.notify)

    async def render_get(self, request):
        payload = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S").encode('ascii')
        return aiocoap.Message(code=aiocoap.CONTENT, payload=payload)


logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)

def main():
    # Resource tree creation
    root = resource.Site()

    root.add_resource(('.well-known', 'core'), resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(('time',), TimeResource())
    asyncio.Task(aiocoap.Context.create_server_context(root))
    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()
