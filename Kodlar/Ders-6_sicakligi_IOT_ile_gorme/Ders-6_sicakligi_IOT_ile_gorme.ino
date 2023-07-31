#include <Deneyap_SicaklikNemOlcer.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "************************************";   // Device ID (İndirilen pdf'ten bakılacak)
const char SSID[]               = "****";                                   // Wifi SSID (isim)
const char PASS[]               = "****";                                   // Wifi şifre
const char DEVICE_KEY[]         = "********************";                   // Secret key (İndirilen pdf'ten bakılacak)


float sicaklik;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(sicaklik, READ, 30 * SECONDS, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
TempHum TempHum;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);
  if(!TempHum.begin(0x70)){                        
      delay(1000);
      Serial.println("I2C bağlantısı başarısız ");
   }
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  // Your code here
  sicaklik = TempHum.getTempValue();
}
