void callback(char* topic, byte* payload, unsigned int length) {

  //Serial.println();

  int value1, value2, value3, value4, value5, value6;
 
  if (strcmp(topic,"Esp8266/sensor")==0){

   char  message[length];

   for (int i=0;i<length;i++) {
      message[i] = (char)payload[i];
      //Serial.print((char)payload[i]);
    }

    //Serial.println();
    
    receivedFromEsp = true;
    
    //Serial.print("Message arrived from topic: ");
    //Serial.println(topic);

    //Serial.print("Message: ");
    //Serial.println(message);

    int n = sscanf(message, "temp:%d,tilt:%d,vibration:%d,heart:%d,distance:%d,call:%d", &value1, &value2, &value3, &value4, &value5, &value6);
    
    //Serial.print("Temperature room: ");
    //Serial.println(value1);
    temperatureEsp = value1;
    
    //Serial.print("Tilt room: ");
    //Serial.println(value2);
    tilt = value2;
  
    //Serial.print("Vibration room: ");
    //Serial.println(value3);
    vibration = value3;

    //Serial.print("Heart room: ");
    //Serial.println(value4);
    heart = value4;

    //Serial.print("Distance room: ");
    //Serial.println(value5);
    distanceEsp = value5;

    //Serial.print("Call from room: ");
    //Serial.println(value6);
    callEsp = value6;

  } 
}
