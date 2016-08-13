const coap    = require('coap') 
const server  = coap.createServer()

server.on('request', function(req, res) {
  console.log(req.url)
  console.log(req.method)
  if (req.method == 'GET' && req.url.split('/')[1] == 'time') {
    res.end(new Date().toISOString())
  }
})

server.listen(function() {
  console.log('server started')
})
