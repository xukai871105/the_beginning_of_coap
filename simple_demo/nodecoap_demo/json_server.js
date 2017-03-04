const coap = require('coap')
const server = coap.createServer()

server.on('request', function(req, res) {
  console.log('method:' + req.method + ' url:' + req.url)
  if (req.method == 'PUT' && req.url.split('/')[1] == 'multi-format') {
    if (req.headers['Accept'] == 'application/json') {
      res.code = '2.04'
      res.setOption('Content-Format', 'application/json')
      res.end(JSON.stringify({payload: req.payload.toString()}))
    } else {
      res.code = '2.04'
      res.end('payload: ' + req.payload.toString())
    }
  }
})

server.listen(function() {
  console.log('server started')
})
