const coap  = require('coap')

var host = '192.168.0.10';
var pathname = 'rgb';

function put_request() {
    var req = coap.request({
        host: host, 
        port: 5683,
        pathname: pathname, 
        method: 'PUT'
    });
    req.setOption("Content-Format", "application/json");

    // 负载
    var payload = {
        red: parseInt(Math.round(10 + (250 - 10) * Math.random())),
        green: parseInt(Math.round(10 + (250 - 10) * Math.random())),
        blue: parseInt(Math.round(10 + (250 - 10) * Math.random()))
    }
    console.log(payload);
    req.end(JSON.stringify(payload));

    req.on('response', function(res) {
        console.log('response code: ' + res.code);
        console.log('response payload: ' + res.payload);
    });
}

setInterval(put_request, 5000);

