#include <WiFi.h>
#include "FirebaseESP32.h"
#include "time.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FIREBASE_HOST "naufalarif-20e6a-default-rtdb.asia-southeast1.firebasedatabase.app" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "AIzaSyAxmczt_l4HYCNTrm7aIOXswdyW3gGyW1Q"
#define WIFI_SSID "Warung Sumber 4G"
#define WIFI_PASSWORD "Ngopidisumber"

#define ONE_WIRE_BUS 33
#define SensorPin 32 
#define DO_PIN 35         //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.8            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
#define VREF 5000    //VREF (mv)
#define ADC_RES 4096 //ADC Resolution
 
//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0
 
#define READ_TEMP (25) //Current water temperature ℃, Or temperature sensor function
 
//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (1810) //mv
#define CAL1_T (25)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
#define CAL2_V (1300) //mv
#define CAL2_T (15)   //℃

OneWire oneWire(33); // Replace pinNumber with the actual pin number connected to your sensor
//DallasTemperature sensors(&oneWire);

const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};
 
uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
uint16_t DO;
uint8_t Relay;
float PH;

int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c)
{
#if TWO_POINT_CALIBRATION == 00
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#endif
}
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

DallasTemperature sensors(&oneWire);

float Celsius = 0;
float Fahrenheit = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4); // Alamat I2C dan ukuran LCD (20x4)

//Define Firebase Data object
FirebaseData firebaseData;
String path = "/database";
String jsonStr;

int timeEpoch;
char timestamp[20];
const char* ntpServer = "pool.ntp.org";
// Timer variables (send new readings every minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 30000;

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

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

void setup(){
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(LED,OUTPUT);
  sensors.begin();
  Serial.begin(115200);
  Serial.println("pH meter experiment!");    //Test the serial monitor

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  configTime(6*3600, 3600, ntpServer);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop(){
    if (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0){
        sendDataPrevMillis = millis();
        
        sensors.requestTemperatures();
        Celsius = sensors.getTempCByIndex(0);
        Temperaturet = (uint8_t)READ_TEMP;
        ADC_Raw = analogRead(DO_PIN);
        ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;

        if(readDO(ADC_Voltage, Temperaturet)<8000){
          Relay = 1;
        }else{
          Relay = 0;
        }

        Serial.print("Temperaturet:\t" + String(Temperaturet) + "\t");
        Serial.print("ADC RAW:\t" + String(ADC_Raw) + "\t");
        Serial.print("ADC Voltage:\t" + String(ADC_Voltage) + "\t");
        Serial.println("DO:\t" + String(readDO(ADC_Voltage, Temperaturet)) + "\t");
      
        static unsigned long samplingTime = millis();
        static unsigned long printTime = millis();
        static float pHValue,voltage;

        if(millis()-samplingTime > samplingInterval){
          pHArray[pHArrayIndex++]=analogRead(SensorPin);
          if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
          voltage = avergearray(pHArray, ArrayLenth)*3.3/4095;
          pHValue = (3.3*voltage)+Offset;
          PH = (3.3*voltage)+Offset;
          samplingTime=millis();
        }

        if(millis() - printTime > printInterval){   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
          Serial.print("Voltage:");
          Serial.print(voltage,2);
          Serial.print("    pH value: ");
          Serial.println(pHValue,2);
          digitalWrite(LED,digitalRead(LED)^1);
          printTime=millis();
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature :");
        lcd.setCursor(16, 0);
        lcd.print(Temperaturet);
        lcd.print((char)223); // Simbol derajat Celsius
        lcd.print("C");
        lcd.setCursor(0, 1);
        lcd.print("pH value    :");
        lcd.setCursor(15, 1);
        lcd.print(pHValue);
        lcd.setCursor(0, 2);
        lcd.print("DO     :");
        lcd.setCursor(10, 2);
        lcd.print(readDO(ADC_Voltage, Temperaturet));
        lcd.print("mg/l");
        delay(1000);

        //Get current timestamp
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
        //Serial.println("Failed to obtain time");
        return;
        }
        strftime(timestamp, 20, "%d-%m-%Y %H:%M:%S", &timeinfo);
        timeEpoch = getTime();
        
        Firebase.pushJSON(firebaseData, path + "/reading/SUHU", String(Temperaturet));
        Firebase.pushJSON(firebaseData, path + "/reading/DO", String(readDO(ADC_Voltage, Temperaturet)));
        Firebase.pushJSON(firebaseData, path + "/reading/PH", String(pHValue));
        Firebase.pushJSON(firebaseData, path + "/reading/RELAY", String(Relay));
        Firebase.pushJSON(firebaseData, path + "/reading/timestamp", String(timeEpoch));

        monitoringValue(String(Temperaturet), String(readDO(ADC_Voltage, Temperaturet)), String(PH), Relay);
    }
}

void monitoringValue(float suhu, int oksigen, float ph, int relay){
  Firebase.setFloat(firebaseData, path + "/monitoring/SUHU", suhu);
  Firebase.setInt(firebaseData, path + "/monitoring/DO", oksigen);
  Firebase.setFloat(firebaseData, path + "/monitoring/PH", ph);
  Firebase.setInt(firebaseData, path + "/monitoring/RELAY", relay);
}