//Send DHT11 Sensor Data to the Firebase Realtime Database
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <DHTesp.h>
#include <Time.h>
#include <NTPtimeESP.h>
DHTesp myDHT;

#include <Wire.h>
#include <SFE_BMP180.h>
SFE_BMP180 bmp180;

#define FIREBASE_HOST "weather-station-d6570.firebaseio.com" //--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "6v6VprmWV4CL1cEtzL1VSwmNW7WfBR3r8EGO59rC" //--> Your firebase database secret code.

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

#define DHTTYPE DHT11 //--> Defines the type of DHT sensor used (DHT11, DHT21, and DHT22), in this project the sensor used is DHT11.
#define DHTPIN 2
 
//SSID and Password of your WiFi router.
//const char* ssid = "DIGI-68C2";
//const char* password = "Zbu2NMNz";
const char* ssid = "Spectral";
const char* password = "andreipr123";
//----------------------------------------

const int DHTPin = 5; //--> The pin used for the DHT11 sensor is Pin D1 = GPIO5
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  myDHT.setup(2,DHTesp::DHT11); // setup the sensor
  delay(500);

  bool success = bmp180.begin();

  if (success) {
    Serial.println("BMP180 connected");
  }
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  //Firebase Realtime Database Configuration.
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //----------------------------------------
}

void loop() {
  //Reading Temperature and Humidity
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  //variables used by the DHT11 sensor
  float t = myDHT.getTemperature();
  float h = myDHT.getHumidity();

  //variables used by the BMP180 sensor
  char status;
  double T, P;
  bool success = false;
  String date;
  String hour;
 
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));
  
    //Send Humidity data to the Firebase Realtime Database.
    Firebase.setInt(("/FirebaseBranch/date_"+date+"/hour_"+hour+"/humidity"),h); //--> Command or code for sending data (Integer data type) to the Firebase Realtime Database.
  
    // Conditions for handling errors.
    if (Firebase.failed()) { 
        Serial.print("setting Humidity failed :");
        Serial.println(Firebase.error());  
        delay(500);
        return;
    }
    //----------------------------------------
  
    //Send Temperature data to the Firebase Realtime Database.
    Firebase.setFloat("temperature",t); //--> Command or code for sending data (Float data type) to Firebase Realtime Database.
  
    // Conditions for handling errors.
    if (Firebase.failed()) { 
        Serial.print("setting Temperature failed :");
        Serial.println(Firebase.error());  
        delay(500);
        return;
    }
    //----------------------------------------
  
    Serial.println("Setting successful");
    Serial.println();
  
    status = bmp180.startTemperature();
    if(status != 0){
      //if the status is different than 0 -> it means that the value returned in the status is 
      //the number of ms needed to wait for the mesurement to be completed.
      delay(status);
      status = bmp180.getTemperature(T);
      
      if (status != 0) {
        status = bmp180.startPressure(3);
  
        if (status != 0) {
          delay(status);
          status = bmp180.getPressure(P, T);
  
          if (status != 0) {
            Serial.print("Pressure: ");
            Serial.print(P,2);
            Serial.println(" hPa");
  
            Serial.print("Temperature: ");
            Serial.print(T,2);
            Serial.println(" C");
  
            //Send Temperature data to the Firebase Realtime Database.
            Firebase.setInt(("/FirebaseBranch/date_"+date+"/hour_"+hour+"/temperature"),T); //--> Command or code for sending data (Integer data type) to the Firebase Realtime Database.
          
            // Conditions for handling errors.
            if (Firebase.failed()) { 
                Serial.print("setting Temperature(BMP180) failed :");
                Serial.println(Firebase.error());  
                delay(500);
                return;
            }
            
            //Send Pressure data to the Firebase Realtime Database.
            Firebase.setInt(("/FirebaseBranch/date_"+date+"/hour_"+hour+"/pressure"),P); //--> Command or code for sending data (Integer data type) to the Firebase Realtime Database.
          
            // Conditions for handling errors.
            if (Firebase.failed()) { 
                Serial.print("setting Barometric Pressure failed :");
                Serial.println(Firebase.error());  
                delay(500);
                return;
            }
          }
        }
      }
    }
  }
  delay(5000);
  
 
  
  
}
