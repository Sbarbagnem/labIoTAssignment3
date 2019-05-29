/*
 * Gruppo: 
 * - Mirko Rima 793435
 * - Samuele Ventura 793060
 * - Luca Virgilio 794866
 *
 * ESP8266
 * SENSOR: - VIBRATION
 *         - TILT
 *         - HEART
 *         - TEMPERATURE
 *         - PROXIMITY
 * 1 BUTTON
 */
 
// Library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <SPI.h>
#include <OneWire.h>

// WiFi settings
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

IPAddress ip(149, 132, 182, 63);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(149, 132, 2, 3);
IPAddress gateway(149, 132, 182, 1);
IPAddress mqtt_server(149, 132, 182, 132); 

WiFiClient espClient;
// Variable to set esp8266 as client to publish on broker
PubSubClient client(espClient);

//Array sensor to send value with MQTT
char val_sensor[100];

//variabile start/off send by MKR1000
boolean invia = false;

// ------------------------------- SENSOR -------------------------------------
//Temperature sensor
//For conversion of raw data to C
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
byte i;
byte present = 0;
byte data[12];
byte addr[8];
double val4;
double Fract_double;
OneWire ds(D3);  // on pin 10
int esp_temp;

//Switch button
int buttonpin = D1; // define the key switch sensor interface
int esp_butt;

//Tilt 
int tilt_pin = D6; // define the percussion Sensor Interface
int val; // define numeric variables val
int esp_tilt;

// Vibration
int vibration_pin = D2; // define the percussion Sensor Interface
int val2; // define numeric variables val
int esp_vibration;

//Heartbeat
int heart_pin = A0;
double alpha = 0.75;
double change = 0.0;
double minval = 0.0;
int esp_heart;
double val3;

//Proximity 
long esp_prox;
int triggerPort = D5;
int echoPort = D7;

boolean sleep;

// ------------------------------- MAIN ------------------------------------

void setup() {
  while(true){//Inizialize serial port
    Serial.begin(9600);
    Serial.setTimeout(2000);
    while (!Serial) { }
    
    Serial.println("Device Started");
    Serial.println("-------------------------------------");
    Serial.println("Running Deep Sleep Program!");
    Serial.println("-------------------------------------");
    
    // define switch sensor output interface
    pinMode (buttonpin, INPUT); 
    // define tilt sensor output interface
    pinMode (tilt_pin, INPUT) ; 
    // define vibration sensor output interface
    pinMode (vibration_pin, INPUT) ; 
    // define proximity sensor output/input interface
    pinMode(triggerPort, OUTPUT);
    pinMode(echoPort, INPUT);
  
    //WiFi connection
    WiFiConnect();
    //MQTT and sensor rilevation
    MQTTsetup();
  
   // Waiting for the start message from MKR1000
   if(invia){
    rileva();
   }
   if(sleep){
    Serial.println("Going into deep sleep for 1 minute");
    ESP.deepSleep(60e6); // 1 minute
   }
  delay(1000); 
  }
  
}

void loop() {
    
}

void MQTTsetup(){
  client.loop();
  // Set property for MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  // reconnect to MQTT
  if (!client.connected()) {
    reconnect();
  }
}

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  if (strcmp(topic,"Arduino/start") == 0){ 
   Serial.print("Message arrived from topic: "); 
   Serial.println(topic); 
   
   char  message[length]; 
 
   for (int i=0;i<length;i++) { 
      message[i] = (char)payload[i]; 
      Serial.print((char)payload[i]); 
    } 
 
    Serial.println(); 
 
    if(message[0] == '0'){
      invia = false; 
    }else{
      invia = true;
    }       
  }

   if (strcmp(topic,"Arduino/sleep") == 0){ 
   Serial.print("Message arrived from topic: "); 
   Serial.println(topic); 
   
   char  message[length]; 
 
   for (int i=0;i<length;i++) { 
      message[i] = (char)payload[i]; 
      Serial.print((char)payload[i]); 
    } 
 
    Serial.println(); 
 
    if(message[0] == '0'){
      sleep = false; 
    }else{
      sleep = true;
    }
       
  }
}

//WiFi Connect
// WiFi configuration and connection test

void WiFiConnect(){
  WiFi.config(ip, dns, gateway, subnet);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("In attesa di connettersi al SSID: ");
    Serial.println(SECRET_SSID);
  
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.println(".");
      delay(10000);
    }
    Serial.println("\nConnesso.");
    // Now you are connected
  }
}
// Reconnect function
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("espClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("Arduino/start");
      client.subscribe("Arduino/sleep");  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//------------------------------ FUNCTION ---------------------------------------

// Temperature sensor 
double temp(){

  if ( !ds.search(addr)) {
      ds.reset_search();
      
  }
  // If you want to know family device uncomment the Serial.print  
  if ( addr[0] == 0x10) {
      //Serial.print("Device is a DS18S20 family device.\n");
  }
  else if ( addr[0] == 0x28) {
      //Serial.print("Device is a DS18B20 family device.\n");
  }
  else {
      //Serial.print("Device family is not recognized: 0x");
      //Serial.println(addr[0],HEX);
      return -1000;
  }
 
  ds.reset();
  ds.select(addr);
  // start conversion, with parasite power on at the end
  ds.write(0x44,1);         
  
  delay(1000);     
  present = ds.reset();
  ds.select(addr);  
  // Read Scratchpad  
  ds.write(0xBE);         
  // we need 9 bytes
  for ( i = 0; i < 9; i++) {           
    data[i] = ds.read();
  }
  //Conversion of raw data to C
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  // test most sig bit
  SignBit = TReading & 0x8000;  
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; 
  }
  Tc_100 = (6 * TReading) + TReading / 4;    
  // separate off the whole and fractional portions
  Whole = Tc_100 / 100;  
  Fract = Tc_100 % 100;
  
  if (SignBit) // If its negative
  {
     Serial.print("-");
  }
  
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print("\n");
  //End conversion to C
  Fract_double =(double)Fract/100;
  val4 = (double)Whole + Fract_double;
  return val4;
}

// Tilt sensor
int tilt(){
  val = digitalRead (tilt_pin);
  return val;
}

// Vibration sensor
int vibration(){
  val2 = digitalRead (vibration_pin);
  return val2;
}

// Heartbeat sensor
double heart() {
  static double oldValue = 0;
  static double oldChange = 0;
  
  float rawValue = analogRead (heart_pin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  val3 = value;
  oldValue = value;
  delay(100);
  return val3; 

}

// Proximity sensor
long prox()
{
  // bring the trigger output low
  digitalWrite( triggerPort, LOW);
  delayMicroseconds(2);
  // send a pulse of 10microsec on trigger
  digitalWrite(triggerPort, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPort, LOW);
  // takes the signal in the echo port as an input and finds a distance (r) via a calculation
  long duration = pulseIn( echoPort, HIGH);
  long r = 0.034 * duration / 2;
  delay(100);
  return r;
}
//--------------------------------- DETECTION AND SENDING ---------------------------------------------

void rileva(){
  esp_butt = digitalRead (buttonpin); 
  esp_temp = (int)temp(); 
  // pulse rate per minute
  esp_heart = (int)(heart()/12); 
  esp_tilt = tilt();
  esp_vibration = vibration();
  esp_prox = prox();

  // Serial print of value sensor
  Serial.println("temp:");
  Serial.println(esp_temp);
  Serial.println();
  Serial.println("tilt:");
  Serial.println(esp_tilt);
  Serial.println();
  Serial.println("vibration:");
  Serial.println(esp_vibration);
  Serial.println();
  Serial.println("heart:");
  Serial.println(esp_heart);
  Serial.println();
  Serial.println("distance:");
  Serial.println(esp_prox);
  Serial.println();

  // String to send via MQTT
  snprintf(val_sensor, 100, "temp:%d,tilt:%d,vibration:%d,heart:%d,distance:%d,call:%d", esp_temp, esp_tilt, esp_vibration, esp_heart, esp_prox, esp_butt);
  // Serial print of the String
  Serial.println(val_sensor);
  delay(2000); 
  // Publish (send data) in a topic so that MKR1000 can receive the sensor values
  client.publish("Esp8266/sensor",val_sensor);
}
