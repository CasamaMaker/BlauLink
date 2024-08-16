#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_sleep.h>

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x88, 0x5D, 0xBD}; //llum //{0xA0, 0x76, 0x4E, 0x36, 0x0A, 0x89};  //PETIT //A0:76:4E:36:0A:89 // BIG  //64:E8:33:C7:62:50

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println(status);
  if(status==0){
    leds[0] = CRGB::Green;
    FastLED.show();
    leds[0] = CRGB::Green;
    FastLED.show();
    /*delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
    Serial.println("green");*/
  }else{
    leds[0] = CRGB::Red;
    FastLED.show();
    leds[0] = CRGB::Red;
    FastLED.show();
    /*delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
    Serial.println("red");*/
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  pinMode(5, INPUT);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);

}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  /*if (result == ESP_OK) {
    Serial.println("Sent with success");
    
  
    leds[0] = CRGB::Blue;
    FastLED.show();
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(500);
  }
  else {
    Serial.println("Error sending the data");
  }*/

  delay(500);
  while(digitalRead(5)){
    leds[0] = CRGB::Blue;
    FastLED.show();
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
  }

  //delay(1500);
  esp_deep_sleep_start();
}