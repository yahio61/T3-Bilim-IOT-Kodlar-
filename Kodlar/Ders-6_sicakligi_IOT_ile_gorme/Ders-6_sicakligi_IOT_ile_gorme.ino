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

  Serial.begin(9600);

  delay(1500);
  if(!TempHum.begin(0x70)){                        
      delay(1000);
      Serial.println("I2C bağlantısı başarısız ");
   }

  initProperties();


  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  sicaklik = TempHum.getTempValue();
}
