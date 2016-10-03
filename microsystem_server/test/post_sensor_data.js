const coap  = require('coap')

// var host = 'localhost'
var host = '192.168.0.6'
var device_id = '12CD'

var temp = Math.round(10 + (20 - 10) * Math.random());
var hum = Math.round(60 + (90 - 60) * Math.random());
var light = Math.round(2000 + (3000 - 2000) * Math.random());
console.log('temp, hum, light', temp, hum, light);

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
