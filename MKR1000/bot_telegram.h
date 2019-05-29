#include <UniversalTelegramBot.h>

// Telegram BOT
const char BotToken[] = "824668381:AAEa7TmxzxDLouxR42FHUt29XUrhW5md6c4"; // Token Bot
String response;
String chat_id;
const int Bot_mtbs = 3000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
String startMessage="Scegliere monitoring system per accedere alla pagina web. Status permette di interagire direttamente con i sensori ";
WiFiSSLClient clientBot;  
UniversalTelegramBot bot(BotToken,clientBot);

// inizializzazione variabili sensori collegati a MKR1000
long distance_bot, light_bot, sound_bot, wifi_bot;
int flame_bot;
float temperature_bot;

void statusMessage(String chat_id){
  getStatusHome();
  Serial.println("send status message");
  response="Temperature: ";
  response.concat(temperature);
  response.concat("\n");
  response.concat("Wifi: ");
  response.concat(wifi);
  response.concat("\n");
  response.concat("Distance: ");
  response.concat(distance);
  response.concat("\n");
  response.concat("Flame: ");
  response.concat(flame);
  response.concat("\n");
  response.concat("Sound: ");
  response.concat(sound);
  response.concat("\n");
  response.concat("light: ");
  response.concat(light);
  response.concat("\n");
  bot.sendMessage(chat_id,response); 
}

void sensorMessage(String chat_id, String sensor, long value){
    Serial.print("----> send value of ");
    Serial.println(sensor);  
    response=sensor;
    response.concat(": ");
    response.concat(value);
    bot.sendMessage(chat_id,response); 
}
void sensorMessageAlert(String chat_id, String sensor, String alert){
    Serial.print("----> send value of ");
    Serial.println(sensor);  
    response = alert;
    bot.sendMessage(chat_id,response); 
}
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    
    /*if (text == "/start") {
        String keyboardJson = "[{ \"text\" : \"get status home system\", \"callback_data\" : \"sensor\" }]";
        bot.sendMessageWithInlineKeyboard(chat_id, startMessage, "", keyboardJson);
      }*/
    
    if (text == "sensor") {
         Serial.println("---new version---");
         String keyboardJson = "[[\"status\"],[\"temperature\", \"wifi\"],[\"sound\", \"flame\"],[\"distance\", \"light\"]]";
         bot.sendMessageWithReplyKeyboard(chat_id, "Status ritorna i valori di tutti i sensori. Altrimenti premere su un singolo sensore", "", keyboardJson, true);
      }

    if (text =="status"){
      statusMessage(chat_id);
      }
     else if (text=="temperature"){
     // temperatureMessage(chat_id);
     sensorMessage(chat_id, "temperature", getTemperature());
      }
     else if (text=="wifi"){
      //wifiMessage(chat_id);
      sensorMessage(chat_id, "wifi", getWifiStatus());
      }
     else if (text=="flame"){
      //flameMessage(chat_id);
      flame_bot = getflame();
      sensorMessage(chat_id, "flame", flame_bot);
      if(flame_bot == 1)
        sensorMessageAlert(chat_id, "flame", errFlame);
      }
     else if (text=="sound"){
      //soundMessage(chat_id);
      sensorMessage(chat_id, "sound", getsound());
      }
     else if (text=="light"){
      //lightMessage(chat_id);
      sensorMessage(chat_id, "light", getlight());
      } 
     else if (text=="distance"){
     //distanceMessage(chat_id);
     sensorMessage(chat_id, "distance", getDistanza());
      }
    }
}

void sendErrorMessage(){
  if(flagVibration){
    bot.sendMessage(chat_id, "pericolo vibrazioni");
    flagVibration = false;
  }
  if(flagTilt){
    bot.sendMessage(chat_id, "pericolo tilt");
    flagTilt = false;
  }
  if(flagHeart){
    bot.sendMessage(chat_id, "pericolo heart");
    flagHeart = false;
  }
  if(flagTempEsp){
    bot.sendMessage(chat_id, "pericolo temperatura");
    flagTempEsp = false;
  }
  if(flagDead){
    bot.sendMessage(chat_id, "pericolo morte");
    flagDead = false;
  }
  if(flagDistance){
    bot.sendMessage(chat_id, "il paziente sta scappando");
    flagDistance = false;
  }
  if(flagCall){
    bot.sendMessage(chat_id, "chiamata dal paziente");
    flagCall = false;
  }
  if(flagWifi){
    bot.sendMessage(chat_id, "wifi basso");
    flagWifi = false;
  }
  if(flagTemp){
    bot.sendMessage(chat_id, "temperatura alta");
    flagTemp = false;
  }
  if(flagIntrusion){
    bot.sendMessage(chat_id, "pericolo intruso");
    flagIntrusion = false;
  }
  if(flagFlame){
    bot.sendMessage(chat_id, "pericolo fiamme");
    flagFlame = false;
  }
  if(flagLight){
    bot.sendMessage(chat_id, "luce bassa");
    flagLight = false;
  }
  if(flagSound){
    bot.sendMessage(chat_id, "suono alto");
    flagSound = false;
  }
}

void botMessage (){
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    sendErrorMessage();

    Bot_lasttime = millis();
  }
}
