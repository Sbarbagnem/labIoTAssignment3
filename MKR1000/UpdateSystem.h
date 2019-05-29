void printOnLcd(int start, int finish, String messaggio, int del, boolean c) {

  lcd.setCursor(start, finish);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.print(messaggio);
  delay(del);
  if (c)
    lcd.clear();
  lcd.setRGB(0, 255, 0);
}

// allarm for MKR1000, on buzzer and led 1
void allarm() {
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledAllarm, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledAllarm, LOW);
  }
}

// allarm for ESP8266, on buzzer and led 2
void allarmEsp() {
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledAllarmEsp, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledAllarmEsp, LOW);
  }  
}

int inArray(int arr[], int value, int element){

  int pos = 0;
  
  for(int i = 0; i < countCycle; i++){
      if(arr[i] == element){
          arr[i] = int(value);
          return i;
      }
  }
  return countCycle;
}

void resetArray(int arr[]){
 
  for(int i = 0; i<countCycle; i++){
    arr[i] = -999;
  }
}

void printArray(int arr[]){

  Serial.println("aggiorno array");
    
  for(int i = 0; i<countCycle; i++){
    Serial.println(arr[i]);
  }
  Serial.println();
}

int meanArray(int arr[]){
  
  int mean = 0;
  
  for(int i = 0; i < countCycle; i++){
    mean += arr[i];
  }
  return  int(mean / countCycle);
}

// update status variables of MKR1000
void updateHome() {

  // temperatura
  if(temperature <= -40){
    flagHardTemp = true;
  } else {
    flagHardTemp = false;
  }
  
  if(!flagHardTemp){
    if (temperature > 30 and countTemp < 2) {
      countTemp ++;
      flagTemp = false;
    } else if (temperature > 30 and countTemp >= 2) {
      allarm();
      // stampa a monitor
      printOnLcd(0, 0, errTemp, 1000, true);
      // set flag for catch event
      flagTemp = true;
      writeDB = true;
    } else if (temperature <= 30 and countTemp > 0) {
      countTemp --;
      flagTemp = false;
      writeDB = false;
    }
  }


  // wifi
  if (wifi > - 25 and countWifi < 2) {
    countWifi ++;
    flagWifi = false;
  } else if (wifi > - 25 and countWifi <= 2) {
    allarm();
    // stampa a monitor
    printOnLcd(0, 0, errWifi, 1000, true);
    flagWifi = true;
    writeDB = true;
  } else if (wifi <= - 25 and countWifi > 0) {
    countWifi --;
    flagWifi = false;
    writeDB = false;
  }

  // distance
  if(distance == 0){
    flagHardDist = true;
  } else {
    flagHardDist = false;
  }

  if(!flagHardDist){
    if (distance < 10 and countIntrusion < 2) {
      countIntrusion ++;
      flagIntrusion = false;
    } else if (distance < 10 and countIntrusion >= 2) {
      allarm();
      // stampa a monitor
      printOnLcd(0, 0, errIntrusion, 1000, true);
      flagIntrusion = true;
      writeDB = true;
    } else if (distance >= 10 and countIntrusion > 0) {
      countIntrusion --;
      flagIntrusion = false;
      writeDB = false;
    }
  }

  // flame
  if (flame == 1 and countFlame < 2) {
    countFlame ++;
    flagFlame = false;
  } else if (flame == 1 and countFlame >= 2) {
    allarm();
    // stampa a monitor
    printOnLcd(0, 0, errFlame, 1000, true);
    flagFlame = true;
    writeDB = true;
  } else if (flame == 0 and countFlame > 0) {
    countFlame --;
    flagFlame = false;
    writeDB = false;
  }

  // light
  if(light < 4){
    flagHardLight = true;
  } else {
    flagHardLight = false;
  }

  if(!flagHardLight){
    if (light < 40 and countLight < 2) {
      countLight ++;
      flagLight = false;
    } else if (light < 40 and countLight >= 2) {
      allarm();
      // stampa a monitor
      printOnLcd(0, 0, errLight, 1000, true);
      flagLight = true;
      writeDB = true;
    } else if (light >= 40 and countLight > 0) {
      countLight --;
      flagLight = false;
      writeDB = false;
    }
  }

  if(sound == 9){
    flagHardSound = true;
  } else {
    flagHardSound = false;
  }

  if(!flagHardSound){
    // sound
    if (sound > 40 and countSound < 2) {
      countSound ++;
      flagSound = false;
    } else if (sound > 40 and countSound >= 2) {
      allarm();
      // stampa a monitor
      printOnLcd(0, 0, errSound, 1000, true);
      flagSound = true;
      writeDB = true;
    } else if (sound <= 40 and countSound > 0) {
      countSound --;
      flagSound = false;
      writeDB = false;
    }
  }
}

// update status variables of ESP8266
void updateEsp(){
  
 // vibration
  if (vibration == 1) {
    flagVibration = true;
    allarmEsp();
  } else {
    flagVibration = false;
  }

  // tilt
  if (tilt == 1) {
    flagTilt = true;
    allarmEsp();
  } else {
    flagTilt = false;
  }

  // heart
  if (((heart > 0 and heart < 10) or heart > 100) and countHeart < 2) {
    countHeart ++;
    flagHeart = false;
  } else if (((heart > 0 and heart < 10) or heart > 100) and countHeart >= 2) {
    flagHeart = true;
    allarmEsp();    
  } else if ((heart >= 10 and heart < 100) and countHeart > 0) {
    countHeart --;
    flagHeart = false;
  }

  // dead 
  if (heart == 0) {
    flagDead = true;
    allarmEsp();
  } else {
    flagDead = false;
  }

 // prox
  if (distanceEsp < 20) {
    flagDistance = true;
    allarmEsp();
  } else {
    flagDistance = false;
  }

  // temp
  if (temperatureEsp > 30 and countTempEsp < 2) {
    countTempEsp ++;
    flagTemp = false;
  } else if (temperatureEsp > 30 and countTempEsp >= 2) {
    flagTemp = true;
    allarmEsp();    
  } else if (temperatureEsp <= 30 and countTempEsp > 0) {
    countTempEsp --;
    flagTemp = false;
  }

  if(callEsp == 0){
    flagCall = true;
  } else{
    flagCall = false;
  }
}

// update array to store values and then make mean
void updateArrayValue(char* sistema){
  
  // aggiungo valore rilevati ad array storici
  // sulla base dei sistemi che stanno rielvando (arduino,esp,entrambi)
  if (strcmp(sistema, "home") == 0){
    
    int index = 0;
    inArray(arr_distance, distance, -999);
    inArray(arr_light, light, -999);
    inArray(arr_sound, sound, -999);
    inArray(arr_wifi, wifi, -999);
    index = inArray(arr_temperature, temperature, -999);
    
    // se ho salvato tot valori dei sensori allora faccio le medie da salvare in DB
    if(index == countCycle){

      distance = meanArray(arr_distance);
      light = meanArray(arr_light);
      wifi = meanArray(arr_wifi);
      temperature = meanArray(arr_temperature);
      sound = meanArray(arr_sound);

      writeDB = true;
      
    }
  }
    
  if (strcmp(sistema, "health") == 0){

    int index = 0;

    inArray(arr_heart, heart, -999);
    index = inArray(arr_temperature_esp, temperatureEsp, -999);
    
    // se ho salvato tot valori dei sensori allora faccio le medie da salvare in DB
    if(index == countCycle){
      heart = meanArray(arr_heart);
      temperatureEsp = meanArray(arr_temperature_esp);
      writeDB = true;
    }
  }
}

void cleanVarHome() {

  // flag event
  flagTemp = false;
  flagWifi = false;
  flagIntrusion = false;
  flagFlame = false;
  flagLight = false;
  flagSound = false;

  countTemp = 0;
  countWifi = 0;
  countIntrusion = 0;
  countFlame = 0;
  countLight = 0;
  countSound = 0;
}
void clearVarEsp(){
  
  flagVibration = false;
  flagTilt = false;
  flagHeart = false;
  flagTempEsp = false;
  flagDead = false;
  flagDistance = false;
  flagCall = false;  

  countHeart = 0;
  countTempEsp = 0;
}

boolean errorFromEsp(){
  if(flagVibration | flagTilt | flagHeart | flagTempEsp | flagDead | flagDistance | flagCall)
    return true;
  return false;
}
