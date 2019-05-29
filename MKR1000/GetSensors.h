float getTemperature() {

  int a = analogRead(pinTempSensor);
  float R = 1023.0 / a - 1.0;
  R = R0 * R;
  temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15;

  return temperature;

}

// Status del WiFi
long getWifiStatus() {
  // prendi la forza del segnale ricevuto
  long rssi = WiFi.RSSI();
  return rssi;
}

// rileva la distanza: sensore di prossimità HC-SR04
long getDistanza()
{
  //porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW);
  delayMicroseconds(2);
  //invia un impulso di 10microsec su trigger
  digitalWrite(triggerPort, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPort, LOW);
  // prende in ingresso il segnale nella porta echo e tramite un calcolo trova distanza (r)
  long duration = pulseIn( echoPort, HIGH);
  long r = 0.034 * duration / 2;
  delay(100);
  return r;
}

// rileva la luce
long getlight()
{
  int light = analogRead(pinLight);
  // mappa i valori della luce in un intervallo da 0 a 10
  light = map(light, 0, 800, 0, 100);
  //light = map(light, 0, 800, 0, 20); // rimappaggio valori per problemi hardware
  delay(100);
  return light;
}

// rileva il suono
long getsound()
{
  long sum = 0;
  for (int i = 0; i < 32; i++)
  {
    sum += analogRead(pinAdc);
  }

  sum >>= 5;
  delay(10);
  return sum;

}

// rileva se ci sono fiamme
int getflame()
{
  val = digitalRead (pinFlame) ;
  // quando rileva la fiamma accende il led verde del sensore
  if (val == HIGH)
  {
    digitalWrite (Led, HIGH);
  }
  else
  {
    digitalWrite (Led, LOW);
  }
  return val;
}

void getStatusHome() {

  // SENSORE TEMPERATURA *****************************************************************************************
  temperature = getTemperature();
  // stampa a seriale
  //Serial.print("temperature = ");
  //Serial.println(temperature);

  // RILEVAZIONE POTENZA WiFi **************************************************************************************
  wifi = getWifiStatus();
  // stampa a seriale
  //Serial.print("potenza del wifi = ");
  //Serial.println(wifi);

  // SENSORE PROSSIMITA' *******************************************************************************************
  distance = getDistanza();
  // stampa a seriale
  //Serial.print("distanza: ");
  //Serial.println(distance);

  // SENSORE DI FLAME DETECTION **************************************************************************************
  flame = getflame();
  // stampa a seriale
  //Serial.print("fiamma: ");
  //Serial.println(flame);

  // SENSORE DI LUCE **************************************************************************************************
  light = getlight();
  // stampa a seriale
  //Serial.print("luce: ");
  //Serial.println(light);

  // SENSORE DI SUONO ************************************************************************************************
  sound = getsound();
  // stampa a seriale
  //Serial.print("suono: ");
  //Serial.println(sound);
}
