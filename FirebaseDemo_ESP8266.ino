
#include <ESP8266WiFi.h>                         
#include <FirebaseArduino.h>
#include<DHT.h>
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
int soilSensor=A0;
// Set these to run example.
#define FIREBASE_HOST "iot-minor-project.firebaseio.com"
#define FIREBASE_AUTH "2WmqndyKx4tm5vONAVWOwyP5lYzSFP9Ty2aDy5Pk"
#define WIFI_SSID "Ravi"
#define WIFI_PASSWORD "ravi1234"

void setup() {
   Serial.begin(9600);
   dht.begin();
   delay(2000);
   pinMode(soilSensor,INPUT);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  //connect to Firebase.
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}



void loop() {
  
  
 ///////////////////////////////////////////////////////////
 float t=dht.readTemperature();
 float h=dht.readHumidity();
 int rawValue=analogRead(soilSensor);
 int perValue=map(rawValue,0,1023,100,0);
 delay(1000);
 Serial.println("Temperature(C*): ");  Serial.println(t);
 Serial.println("Humidity : ");  Serial.println(h);
 Serial.println("moisture : ");  Serial.println(perValue);
 Serial.println("*******************");
  // update value
  if(!isnan(t)) 
  {
   Firebase.setFloat("DHT11&SoilSensor/Temperature", t);
  }
  if(!isnan(h)) 
  {
  
    Firebase.setFloat("DHT11&SoilSensor/Humidity", h);
  }
  if(!isnan(perValue)) 
  {
    Firebase.setInt("DHT11&SoilSensor/Moisture", perValue);
  }
  //Firebase.setInt("soilSensor/Moisture", perValue);
  if (Firebase.failed()) {
      //Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  

  // get value 
  //Serial.print("Temperature: ");
  //Serial.println(Firebase.getFloat("DHT11/Temperature"));
  //Serial.print("Humditity: ");
  //Serial.println(Firebase.getFloat("DHT11/Humidity"));
  


}

