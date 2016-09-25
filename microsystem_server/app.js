//模块依赖
var express = require('express')
var bodyParser = require('body-parser')
var path = require('path')
var swig = require('swig')
var coap = require('coap')

var routes = require('./routes/index');
var coap_routes = require('./routes/coap_index');

var app = express();

// 设置渲染引擎
app.engine('html', swig.renderFile);
app.set('view engine', 'html');

app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

var coap_port = 5683;
var port = 8090;
var host = '0.0.0.0';

app.use('/', routes);

// 启动http服务器
var http_server = app.listen(port, host)
console.log('Http Server Listening on :' +  host + ':' + port);

// 启动coap服务器
coap.createServer(coap_routes).listen(coap_port);
console.log('CoAP Server Listening on :' + host + ':' + coap_port);