#include <Arduino.h>
#include <WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include <Deneyap_Role.h>

#define WIFI_SSID         "**************"
#define WIFI_PASS         "**************"


#define APP_KEY           "*******************"
#define APP_SECRET        "************************************************************************"
#define SWITCH_ID_1       "*******************"


#define BAUD_RATE         115200


Relay Role;

bool onPowerState1(const String &deviceId, bool &state) {
 Serial.printf("Device 1 turned %s\n", state?"on":"off");
 //Role.RelayDrive(state?HIGH:LOW);
 digitalWrite(4, !(state?HIGH:LOW));
 return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
    
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);
  
  
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
   
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  if(!Role.begin(0x0C)){                           
      delay(3000);
      Serial.println("Röle I2C bağlantısı başarısız "); 
   }
  pinMode(4,OUTPUT);
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
