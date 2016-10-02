// 模块依赖
var coap = require('coap')
// 引入coap路由
var coap_routes = require('./routes/coap_index');

var host = '0.0.0.0';
var coap_port = 5683;

// 启动coap服务器
coap.createServer(coap_routes).listen(coap_port);
console.log('CoAP Server Listening on :' + host + ':' + coap_port);