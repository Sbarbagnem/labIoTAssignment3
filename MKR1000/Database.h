// lunghezza query
char query[512];
// query da inviare al database
char INSERT_DATA[] = "INSERT INTO `iot01`.`sensor`"
                    " (`temperature`, `distance`,`sound`, `light`,`flame`, `WiFi`, `tiltEsp`, "
                      "`vibrationEsp`, `heartEsp`, `distanceEsp`, `callEsp`, `temperatureEsp`, `type`)"
                      " VALUES ('%0.2f', '%d', '%d', '%d', %d, '%d', '%d', '%d', '%d', '%d', '%d', '%0.2f', '%s')";

int writeMultiToDB(float field1, int field2, int field3, int field4, int field5, int field6, int field7, int field8, int field9, int field10, int field11, float field12, String field13) {

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  if(field13 == "MKR1000error"){
    sprintf(query, INSERT_DATA, field1, field2, field3, field4, field5, field6, field7, field8, field9, field10, field11, field12, "MKR1000error"); 
  }
  if(field13 == "MKR1000"){
    sprintf(query, INSERT_DATA, field1, field2, field3, field4, field5, field6, field7, field8, field9, field10, field11, field12, "MKR1000"); 
  }
  if(field13 == "ESP8266error"){
    sprintf(query, INSERT_DATA, field1, field2, field3, field4, field5, field6, field7, field8, field9, field10, field11, field12, "ESP8266error"); 
  }
  if(field13 == "ESP8266"){
    sprintf(query, INSERT_DATA, field1, field2, field3, field4, field5, field6, field7, field8, field9, field10, field11, field12, "ESP8266"); 
  }
  if(field13 == "error"){
    sprintf(query, INSERT_DATA, field1, field2, field3, field4, field5, field6, field7, field8, field9, field10, field11, field12, "error"); 
  }
  if(field13 == "MKR1000-ESP8266"){
    sprintf(query, INSERT_DATA, field1, field2, field3, field4, field5, field6, field7, field8, field9, field10, field11, field12, "MKR1000-ESP8266"); 
  }
  
  Serial.println(query);
  
  // Esegui la query
  cur_mem->execute(query);
  delete cur_mem;

}

// write only values of MKR1000
void updateDBHome(String type){
  writeMultiToDB(temperature, distance, sound, light, flame, wifi, -1000, -1000, -1000, -1000, -1000, -1000, type);
}

// write only values of ESP8266
void updateDBHealth(String type){
    writeMultiToDB(-1000, -1000, -1000, -1000, -1000, -1000, tilt, vibration, heart, distanceEsp, callEsp, temperatureEsp, type);  
}

// write values from both system
void updateDB(String type){
  if(receivedFromEsp){
    writeMultiToDB(temperature, distance, sound, light, flame, wifi, tilt, vibration, heart, distanceEsp, callEsp, temperatureEsp, type);
  } 
  // if ESP8266 don't send values for some error write only MKR1000
  else {
    updateDBHome(type);
  }
}
