var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var http = require('http');

var app = express();
const server = http.createServer(app);
server.listen(8080);

const io = require('socket.io')(server);

// connecting with the serial port of arduino
const SerialPort = require('serialport'); 
const Readline = SerialPort.parsers.Readline;
//set the right port
const port = new SerialPort('COM13', {baudRate: 9600, autoOpen: true});
// set the parser
// read every new line
const parser = port.pipe(new Readline({delimiter: '\r\n'}));

var spawn = require('child_process').spawn;

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');

app.get('/', function(req, res, next) {
    res.render('index');
  });
  
app.get('/home', (req, res)=>{
    res.render('home');
});

app.get('/health', (req, res) =>{
    res.render('health');
});

app.get('/all', (req, res) =>{
    res.render('all');
});

// Read the port data
port.on('open', () => {
    console.log('serial port open');
});

app.post('/onMKR', (req, res) =>{

    console.log("post on MKR ricevuta");

    port.write('onMKR', (err) =>{
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');    
    });    
});

app.post('/onESP', (req, res) =>{

    console.log("post on ESP ricevuta");

    port.write('onESP', (err) =>{
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');    
    });
});

parser.on('data', (data)=> {
    try {
        var str = JSON.parse(data); 
        console.log("dati da arduino");
        console.log(str);
        if(str.system == "MKR"){
            io.emit("valueMkr", str);
        } else if(str.system == "ESP"){
            io.emit("valueEsp", str);
        }
      } catch (e) {
        console.log(data);
      }
});

app.post('/offMKR', (req, res) =>{

    console.log("post off MKR ricevuta");

    port.write('offMKR', (err) =>{
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');    
    });
});

app.post('/offESP', (req, res) =>{

    console.log("post off ESP ricevuta");

    port.write('offESP', (err) =>{
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');    
    });
});

app.post('/onSleep', (req, res) =>{

    console.log("post off MKR ricevuta");

    port.write('onSleep', (err) =>{
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');    
    });
});

app.post('/offSleep', (req, res) =>{

    console.log("post off MKR ricevuta");

    port.write('offSleep', (err) =>{
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');    
    });
});

app.post('/showChartTemp', (req, res) =>{
    console.log("mostra grafico");
});

module.exports = app;
