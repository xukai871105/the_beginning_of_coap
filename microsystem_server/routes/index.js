var express = require('express');
var router = express.Router();
var mysql = require('mysql');

var config = require('../config/db-config.json');

router.get('/index', function (req, res) {
	res.render('index.html', {title : 'Hello'});
});

router.get('/test', function (req, res) {

	var url = '/devices/data.json';
	var conn = mysql.createConnection(config);
	conn.connect();

	conn.query("SELECT DISTINCT device_id FROM sensor_history",
		function(err, rows) {
		if (!err) {
			res.render('test.html', {url: url, title : 'Test', devices: rows});
		}
	});
	
	conn.end(function(err) {
		if (!err) {
			console.log("db close");
		}
	});	
});

router.get('/', function (req, res) {

	get_device_list(function(rows){
		var device_id = rows[0].device_id
		var url = '/devices/' + device_id + '/data.json';
		res.render('admin.html', {url: url, device_id: device_id, devices: rows});		
	})
});

router.get('/devices/:device_id/page', function (req, res) {

	var device_id = req.params.device_id;
	var url = '/devices/' + device_id +'/data.json';

	get_device_list(function(rows){
		res.render('admin.html', {url: url, device_id: device_id, devices: rows});	
	})	
});

router.get('/devices/:device_id/data.json', function (req, res) {
	console.log('------------------------------');
	console.log('get device json');
	var device_id = req.params.device_id;
	var offset = parseInt(req.query.offset) || 0;
	var limit = parseInt(req.query.limit) || 10;
	var total = 0;
	var rows = null;

	console.log('device_id: ' + device_id);
	console.log('offset: ' + offset);
	console.log('limit: ' + limit);

	var conn = mysql.createConnection(config);
	conn.connect(function(err) {
		console.log('connected as id ' + conn.threadId);
	});
	
	conn.query('SELECT COUNT(*) as total FROM sensor_history WHERE device_id=?', 
		[device_id],
		function(err, result) {
			if (!err) {
				total = result[0].total;
				console.log("total: " + total);
			}
	});

	conn.query('SELECT * FROM sensor_history WHERE device_id=? order by id desc limit ?, ?', 
		[device_id, offset, limit],
		function(err, result) {
			if (!err) {
				rows = result;
			} else {
				console.log(err.message);
			}
	});
	
	conn.end(function(err){
		if (!err) {
			console.log("db close");
			res.json({total:total, rows:rows});
		}
	});
});

function get_device_list(callback) {
	var conn = mysql.createConnection(config);
	conn.connect();

	conn.query("SELECT DISTINCT device_id FROM sensor_history",
		function(err, rows) {
		if (!err) {
			callback(rows);
		}
	});
	
	conn.end();	
}
module.exports = router;
