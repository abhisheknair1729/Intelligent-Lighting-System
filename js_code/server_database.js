//code to set up a server using node.js

var express = require('express');
var app = express();

const sqlite3 = require('sqlite3').verbose();

let db = new sqlite3.Database('/home/abhishek/cfi/iLighting/server_code/sensordata.db');
db.run('CREATE TABLE IF NOT EXISTS sensordata(timestamp INT,Data INT)'); //create table with two columns


app.get('/', function (req, res) { //if no data is sent by the NodeMcu
  res.send("No Data Sent");
});

app.get('/:data', function(req, res){  //if data is sent by the nodemcu
  var data = req.params.data; //get data from the http request
  res.send("Data Recieved");
  console.log(data);    //display data on terminal
	
  var d = new Date();
  var timestamp = [d.getFullYear(), d.getMonth(), d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds()] ;  //complete date
  var s = db.prepare('INSERT INTO sensordata VALUES(?,?)');   //prepare the statement, execute it below
  s.run(timestamp[2],data); //store only the date(1-31) in the database
  s.finalize();
});

var port = 8080;
app.listen(port, function () {   //listen at the port 8080
  console.log('Port for the node mcu open at:%s',port);
});


