#ifndef VARIABLES_H
#define VARIABLES_H

// define string error event
#define errTemp "High temperature"
#define errWifi "Low connection"
#define errIntrusion "Intrusion detected"
#define errFlame "Flame detected"
#define errLight "Low light"
#define errSound "High sound"

// define chart array to get alert from esp
char vibration_alert[] = "vibration_alert";
char tilt_alert[] = "tilt_alert";
char heart_alert[] = "heart_alert";
char temp_alert[] = "temp_alert";
char dead_alert[] = "dead_alert";
char distance_alert[] = "distance_alert";

// define char array to get stop alert from esp
char not_vibration_alert[] = "stop_vibration_alert";
char not_tilt_alert[] = "stop_tilt_alert";
char not_heart_alert[] = "stop_heart_alert";
char not_temp_alert[] = "stop_temp_alert";
char not_dead_alert[] = "stop_dead_alert";
char not_distance_alert[] = "stop_distance_alert";

// define flag to catch error value from MKR1000
boolean flagWifi = false;
boolean flagTemp = false;
boolean flagIntrusion = false;
boolean flagFlame = false;
boolean flagLight = false;
boolean flagSound = false;

// define flag to catch error value from ESP8266
boolean flagVibration = false;
boolean flagTilt = false;
boolean flagHeart = false;
boolean flagTempEsp = false;
boolean flagDead = false;
boolean flagDistance = false;
boolean flagCall = false;

// variables for hardware problem
boolean flagHardTemp = false;
boolean flagHardSound = false;
boolean flagHardLight = false;
boolean flagHardDist = false;


// define count to control error value
int countTemp = 0;
int countWifi = 0;
int countIntrusion = 0;
int countFlame = 0;
int countLight = 0;
int countSound = 0;

int countHeart = 0;
int countTempEsp = 0;

// count to write on DB every 6 cycle
const int countCycle = 12;

// array to store value from MKR1000 and then save on DB only the mean
int arr_distance[countCycle]; 
int arr_light[countCycle]; 
int arr_sound[countCycle]; 
int arr_wifi[countCycle]; 
int arr_temperature[countCycle];

// array to store value from ESP8266 and then save on DB only the mean
int arr_heart[countCycle]; 
int arr_temperature_esp[countCycle]; 

// variable for value from sensor on MKR1000
long distance, light, sound, wifi;
int flame;
float temperature;

// variable for value from sensor on ESP8266
int tilt=0, vibration=0, heart=0, distanceEsp=0, callEsp=0;
float temperatureEsp=0.0;

// network connection
IPAddress ip(149, 132, 182, 70);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(149, 132, 2, 3);
IPAddress gateway(149, 132, 182, 1);
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

// client used for WebPage
WiFiClient  client;

// MySQL connection
IPAddress server_addr(149, 132, 182 , 142);
IPAddress mqtt_addr(149, 132, 182 , 132);
char user[] = MYSQL_USER;
char password[] = SECRET_MYSQL_PASS;
WiFiClient  clientDB;
MySQL_Connection conn((Client *)&clientDB);

// MQTT connection
WiFiClient  clientArduino;
PubSubClient clientPubSub(clientArduino);

// to set arduino as WebServer listening on port 80
WiFiServer server(80);

// variabili per il sensore di temperatura con grove
// B value of the thermistor
const int B = 4275;
// R0 = 100k
const int R0 = 100000;
const int pinTempSensor = A0;

//sensore di suono
const int pinAdc = A5;

// sensore di prossimità
int triggerPort = 6;
int echoPort = 7;

//flame sensor KY-026
// definisci l'interfaccia del LED
int Led = 13 ;
// definisci sensore digitale
int pinFlame = 1;
// define la variabile numerica val
int val ;
// variabile per leggere il valore analogico
float sensor;

int buzzer = 3;
int ledAllarm = 4; 
int ledAllarmEsp = 5;

int pinLight = A3;

// display
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
rgb_lcd lcd;

// variable to know if ESP8266 send message
boolean receivedFromEsp = false; 

// variable to write or not on DB
boolean writeDB = false;

const int capacity = JSON_OBJECT_SIZE(7);
const int capacity_all = JSON_OBJECT_SIZE(12);

String inData = "";
boolean onHome = false;
boolean onEsp = false;
boolean sleepMode = false;

String jsonString = "";

StaticJsonBuffer<capacity> jsonBuffer;
#endif
