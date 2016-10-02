// 模块依赖
var express = require('express')
var bodyParser = require('body-parser')
var path = require('path')
var swig = require('swig')
var routes = require('./routes/index');

var app = express();
app.engine('html', swig.renderFile);
app.set('view engine', 'html');
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

var port = 8090;
var host = '0.0.0.0';

app.use('/', routes);

// 启动http服务器
var http_server = app.listen(port, host)
console.log('Http Server Listening on :' +  host + ':' + port);
