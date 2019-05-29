void sendStatusHome(){

    /*//StaticJsonBuffer<capacity> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();

    root["system"] = "MKR";
    root["temperature"] = temperature;
    root["light"] = light;
    root["flame"] = flame;
    root["wifi"] = wifi;
    root["sound"] = sound;
    root["distance"] = distance;
  
    Serial.println();
    root.prettyPrintTo(Serial);*/

    // form a JSON-formatted string:
    jsonString = "{\"system\":\"MKR\"";
    jsonString += ",\"temperature\":\"";
    jsonString += temperature;
    jsonString += "\",\"light\":\"";
    jsonString += light;
    jsonString += "\",\"flame\":\"";
    jsonString += flame;
    jsonString += "\",\"wifi\":\"";
    jsonString += wifi;
    jsonString += "\",\"sound\":\"";
    jsonString += sound;
    jsonString += "\",\"distance\":\"";
    jsonString += distance;  
    jsonString += "\"}";

    Serial.println(jsonString);

    jsonString = "";
}

void sendStatusHealth(){

    // form a JSON-formatted string:
    jsonString = "{\"system\":\"ESP\"";
    jsonString += ",\"vibrazione\":\"";
    jsonString += vibration;
    jsonString += "\",\"tilt\":\"";
    jsonString += tilt;
    jsonString += "\",\"distanza\":\"";
    jsonString += distanceEsp;
    jsonString += "\",\"chiamata\":\"";
    jsonString += callEsp;
    jsonString += "\",\"temperatura\":\"";
    jsonString += temperatureEsp;
    jsonString += "\",\"cuore\":\"";
    jsonString += heart;  
    jsonString += "\"}";

    Serial.println(jsonString);

    jsonString = "";
}

void sendStatusAll(){

    // form a JSON-formatted string:
    jsonString = "{\"system\":\"ESP\"";
    jsonString += ",\"temperature\":\"";
    jsonString += temperature;
    jsonString += "\",\"light\":\"";
    jsonString += light;
    jsonString += "\",\"flame\":\"";
    jsonString += flame;
    jsonString += "\",\"wifi\":\"";
    jsonString += wifi;
    jsonString += "\",\"sound\":\"";
    jsonString += sound;
    jsonString += "\",\"distance\":\"";
    jsonString += distance;  
    jsonString += "\"}";

    Serial.println(jsonString);

    jsonString = "";
}

void receiveData(){

    while (Serial.available() > 0) {
        char received = Serial.read();
        inData.concat(received);
    }
    
    if(inData.equals("onMKR")){
      
      onHome = true;
      onEsp = false;
      
    } else if(inData.equals("offMKR")){
      
      onHome = false;
      
    }

    if(inData.equals("onESP")){
      
      onEsp = true;
      onHome = false;
      
    } else if(inData.equals("offESP")){
      
      onEsp = false;
      
    }

    if(inData.equals("onSleep")){
      
      sleepMode = true;
      clientPubSub.publish("Arduino/sleep", "1");
      
    } else if(inData.equals("offSleep")){
      
      clientPubSub.publish("Arduino/sleep", "0");
      sleepMode = false;
      
    }
    
    
    inData = "";
}
