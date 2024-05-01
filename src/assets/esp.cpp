#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"
#include "time.h"

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID  "Warung Sumber 5G"
#define WIFI_PASSWORD "Ngopidisumber"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAxmczt_l4HYCNTrm7aIOXswdyW3gGyW1Q"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "naufalarif-20e6a-default-rtdb.asia-southeast1.firebasedatabase.app"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
bool signupOK = false;

// Database main path (to be updated in setup with the user UID)
String databasePath;
String parentPath;

// Database child nodes
String sensor1 = "/sensor1";
String sensor2 = "/sensor2";
String sensor3 = "/sensor3";
String sensor4 = "/sensor4";
String timePath = "/timestamp";

int timeEpoch;
char timestamp[20];
const char* ntpServer = "pool.ntp.org";

// Timer variables (send new readings every minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(pinBUZZ, HIGH);
    delay(1000);
  }
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void monitoringValue();

void setup(){
  Serial.begin(9600);
  initWiFi();
  configTime(6*3600, 3600, ntpServer);
  
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    signupOK = true;
  }
  else{
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  databasePath = "/readings";
}

void loop(){
  if (isnan(Temp) || isnan(Hum)) {
    return;
  }
      
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    //Get current timestamp
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      //Serial.println("Failed to obtain time");
      return;
    }
    strftime(timestamp, 20, "%d-%m-%Y %H:%M:%S", &timeinfo);
    timeEpoch = getTime();

    //Send to database    
    parentPath = databasePath + "/" + String(timeEpoch);
    json.set(sensor1.c_str(), String(random(6.0,7.0)));
    json.set(sensor2.c_str(), String(random(4.0,7.0)));
    json.set(sensor3.c_str(), String(random(5.0,7.0)));
    json.set(sensor4.c_str(), String(random(7.0,7.0)));
    json.set(phPath.c_str(), String(random(1.0,7.0)));
    json.set(timePath, String(timestamp));
    Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
    
    monitoringValue();
    
  }
}

void monitoringValue(){
  Firebase.RTDB.setFloat(&fbdo, "Node2/Sensor1/Nilai", random(6.0,7.0));
  Firebase.RTDB.setFloat(&fbdo, "Node2/Sensor2/Nilai", random(6.0,7.0));
  Firebase.RTDB.setFloat(&fbdo, "Node2/Sensor3/Nilai", random(6.0,7.0));
  Firebase.RTDB.setFloat(&fbdo, "Node2/Sensor4/Nilai", random(6.0,7.0));
  Firebase.RTDB.setFloat(&fbdo, "Node2/Sensor5/Nilai", random(6.0,7.0));
}