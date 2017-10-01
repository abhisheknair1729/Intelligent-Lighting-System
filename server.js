//prakhars code nott used for openhouse

var express = require('express');
var morgan = require('morgan');
var path = require('path');
var http = require('http')

var app = express();
app.use(morgan('combined'));

app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname, 'ui', 'index.html'));
});

app.get('/ui/style.css', function (req, res) {
  res.sendFile(path.join(__dirname, 'ui', 'style1.css'));
});

app.get('/ui/madi.png', function (req, res) {
  res.sendFile(path.join(__dirname, 'ui', 'madi1.png'));
});
app.get('/:data', function(req, res){
	var data = req.params.data;
	console.log(data);
	console.log("data received");
	res.sendFile(path.join(__dirname, 'ui', 'index.html'));
});





var port = 8080;
app.listen(port, function () {
  console.log(`Port for the node mcu open at ${port}!`);
});
