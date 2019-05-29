#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <SPI.h>
#include <WiFi101.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <PubSubClient.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h"
#include "Variables.h"
#include "Database.h"
#include "UpdateSystem.h"
#include "GetSensors.h"
#include "mqtt.h"
#include "nodeComunication.h"

void setup() {
  
  // inizializza porta seriale
  Serial.begin(9600);

  lcd.begin(16, 2);
  
  // set up pin ditsance sensor
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
  
  // set up pin flame sensor
  pinMode (Led, OUTPUT);
  pinMode (pinFlame, INPUT);
  
  //set up pin buzzer
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  
  //set up pin led for show allarm
  pinMode(ledAllarm, OUTPUT);

  // reset array that store values from sensor
  resetAllArray();

  // starting message on lcd
  lcd.setCursor(0, 0);
  lcd.print("ASSIGNMENT 2:");
  lcd.setCursor(0, 1);
  lcd.print("HOME MONITORING");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RIMA - VENTURA");
  lcd.setCursor(2, 1);
  lcd.print("VIRGILIO");
  delay(3000);
  lcd.clear();
  lcd.setRGB(0, 255, 0);

  // set value for connection to net
  WiFi.config(ip, dns, gateway, subnet);

  // set up  broker, port and calback function for MQTT connection
  clientPubSub.setServer(mqtt_addr, 1883);
  clientPubSub.setCallback(callback); // callback function to get message from subscribe topic
  clientPubSub.subscribe("Esp8266/sensor");
}

void loop() {

  clientPubSub.loop();
  
  connectToNet();
  
  connectToMysql();
    
  //listenForEthernetClients();

  while(!clientPubSub.connected()){
    reconnect();
  }
  
  //botMessage();

  receiveData();

  /*if(deepMode){ // se da node (webapp) ho attivato deep mode invio su topic a ESP
    clientPubSub.publish("Esp/deepMode", "1");
  } else {
    clientPubSub.publish("Esp/deepMode", "0");
  }*/

  checkToSend();

  delay(1000); // invia dati ogni 2 secodni circa

}

void checkToSend(){

  if(sleepMode){
    clientPubSub.publish("Arduino/sleep", "1");
  } else {
    clientPubSub.publish("Arduino/sleep", "0");
  }

  if(onHome && onEsp){
    sendAll();
  }

  else if(onHome){
    sendHome();
  }

  else if(onEsp){
    sendHealth();
  }

  else {
    stopAll();
  }
}

void sendAll(){

  clientPubSub.publish("Arduino/start", "1");

  getStatusHome();

  updateHome();

  if(receivedFromEsp){
    updateEsp(); 
    sendStatusAll();
    if(writeDB || errorFromEsp()){
      updateDB("error");
    }
    else{
      updateArrayValue("home");
      updateArrayValue("health");
      if(writeDB){
        updateDB("MKR1000-ESP8266"); 
        resetAllArray(); 
        writeDB = false;
      }
    }
  } else {
    sendStatusHome();
    updateDBHome();
  }
}
void stopAll(){

  clientPubSub.publish("Arduino/start" ,"0");
  receivedFromEsp = false;
  clearVarEsp();
  cleanVarHome();
  resetAllArray();
}

void sendHome(){

  clientPubSub.publish("Arduino/start", "0");

  getStatusHome();

  updateHome();

  sendStatusHome();
    
  updateDBHome();

}

void sendHealth(){

  clientPubSub.publish("Arduino/start", "1");

  if(receivedFromEsp){
    updateEsp(); 
    sendStatusHealth();
    updateArrayValue("health");
  }
  if(errorFromEsp()){ 
    updateDBHealth("ESP8266allert");
  }
  if(writeDB) { 
    updateDBHealth("ESP8266"); 
    resetAllArray();
  }   
  writeDB = false;
}

void updateDBHome(){

  if(writeDB){ 
    updateDBHome("MKR1000allert");
  }
  else{ 
    updateArrayValue("home"); 
    if(writeDB){
      updateDBHome("MKR1000"); 
      resetAllArray(); 
    }
  }
  writeDB = false;
}

void connectToNet(){

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("In attesa di connettersi al SSID: ");
    Serial.println(SECRET_SSID);
  
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.println(".");
      delay(10000);
    }
    
    Serial.println("\nConnesso alla rete.");
  
    server.begin();
    //Serial.println("Server Arduino avviato.");
    
    printWifiStatus();
  }
}

boolean connectToMysql(){
 
  if (conn.connect(server_addr, 3306, user, password)){
    delay(1000);
    //Serial.print("Connesione a Mysql riuscita");
  }
  else {
    //Serial.println("Connessione a Mysql fallita.");
  } 
}

void reconnect() {
  
  while (!clientPubSub.connected()) {
    
    //Serial.print("Attempting MQTT connection...");
    
    if (clientPubSub.connect("clientArduino")) {
      //Serial.println("connected");
      clientPubSub.subscribe("Esp8266/sensor");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(clientPubSub.state());
      //Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void printWifiStatus() {

  //Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  //Serial.print("Indirizzo IP: ");
  //Serial.println(ip);

  long rssi = WiFi.RSSI();
  //Serial.print("potenza del segnale (RSSI):");
  //Serial.print(rssi);
  //Serial.println(" dBm");
}

void resetAllArray(){
  resetArray(arr_heart); 
  resetArray(arr_distance);
  resetArray(arr_light);
  resetArray(arr_sound);
  resetArray(arr_wifi);
  resetArray(arr_temperature); 
  resetArray(arr_temperature_esp);
}
