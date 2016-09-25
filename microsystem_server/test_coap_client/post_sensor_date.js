const coap  = require('coap')

var host = 'localhost'
//host = '123.59.83.91'
var device_id = '12CF'

var temp = 14
var hum = 90
var light = 1721

var pathname = 'devices/' +  device_id;
var req = coap.request({
    host: host, 
    pathname: pathname, 
    method: 'POST'
});
req.setOption("Content-Format", "application/json");

var payload = {
	temp: temp,
	hum: hum,
	light: light
}
req.write(JSON.stringify(payload));

req.on('response', function(res) {
    console.log('response code: ' + res.code)
    console.log('response payload: ' + res.payload)
})

req.end()
