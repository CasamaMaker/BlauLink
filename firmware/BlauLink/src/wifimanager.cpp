#include <wifimanager.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"


//******************** INITIALIZE SPIFFS ***********************
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    //Serial.println("An error has occurred while mounting SPIFFS");
  }
  //Serial.println("SPIFFS mounted successfully");
}


//******************** READ FILE FROM SPIFFS ***********************
String readFile(fs::FS &fs, const char * path){
  //Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    //Serial.println("- failed to open file for reading");
    return String(); 
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}


//******************** WRITE FILE TO SPIFFS ***********************
void writeFile(fs::FS &fs, const char * path, const char * message){
  //Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    //Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    //Serial.println("- file written");
  } else {
    //Serial.println("- frite failed");
  }
}


//******************** INITIALIZE WIFI ***********************
bool initWiFi(String ssid, String pass, unsigned long previousMillis, const long interval) {
  if(ssid==""){// || ip==""){
    //Serial.println("Undefined SSID");
    return false;
  }

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid.c_str(), pass.c_str());
  //Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      //Serial.println("Failed to connect.");
      return false;
    }
  }

  //Serial.println(WiFi.localIP());
  return true;
}