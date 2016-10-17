const coap    = require('coap') 
const server  = coap.createServer()

server.on('request', function(req, res) {
  console.log('method:' + req.method + ' url:' + req.url)
  if (req.method == 'GET' && req.url.split('/')[1] == 'time') {
    console.log(new Date().toLocaleString())
    res.end(new Date().toLocaleString())
  }
})

server.listen(function() {
  console.log('server started')
})
