/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define UBM ubm

#define BLYNK_TEMPLATE_ID "TMPL7rZ_6hs9"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "W4ThbbE0s842teVS1V7VumUdi0fuL58k"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Deneyap_ToprakNemiOlcer.h>                    // Deneyap_ToprakNemiOlcer kütüphanesi eklenmesi (Soil Moisture Sensor TL555Q)
#include <Deneyap_SicaklikNemOlcer.h>                   // Deneyap_SicaklikNemOlcer kütüphane eklenmesi
#include <Deneyap_Role.h>                               // Deneyap_Role.h kütüphanesi eklenmesi

#define period 1000                                     // Verilerin gonderilme sıklığı (ms)

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
#ifdef UBM
char ssid[] = "Bilim Uskudar";
char pass[] = "BUmth2023";

#else
char ssid[] = "Yahio";
char pass[] = "345534552";
#endif

SoilMoisture ToprakNemi;                              // SoilMoisture için class tanımlaması
TempHum TempHum;                                      // TempHum için class tanımlamas
Relay Role;                                           // Relay için class tanımlanması
BlynkTimer timer;

unsigned long zaman = 0;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  int veri = param.asInt();
  Serial.print("düğme: ");
  Serial.println(veri);

  if(veri == 0)
     Role.RelayDrive(0);
  else
    Role.RelayDrive(1);     
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
}

void setup()
{
  pinMode(D8,INPUT);
  pinMode(D10,OUTPUT);
  Serial.begin(115200);

  if (!ToprakNemi.begin(0x0F))                                          // begin() fonksiyonu ile nem sensörü haberleşmesi başlatılması
  {
        delay(300);
        Serial.println("Toprak nemi I2C bağlantısı başarısız ");        // I2C bağlantısı başarısız olursa seri monitore yazdırılması
  }
  if(!TempHum.begin(0x70))                                              // begin() fonksiyonu ile sicaklik ve hava nemi sensörü haberleşmesi başlatılması
  {                        
    delay(300);
    Serial.println("Sicaklik ve hava nemi I2C bağlantısı başarısız ");  // I2C bağlantısı başarısız olursa seri terminale yazdırılması
  }
  if(!Role.begin(0x0C))                                                 // begin() fonksiyonu ile relay haberleşmesi başlatılması 
  {                          
      delay(3000);
      Serial.println("Relay I2C bağlantısı başarısız ");                // I2C bağlantısı başarısız olursa seri monitore yazdırılması
  }

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  uint16_t nemDeger = ToprakNemi.ReadSoilMoisture(); // Nem değeri okunması
  nemDeger = map(nemDeger, 740, 860, 100, 0);  //min 750  / max 850
  Serial.print("Toprak Nemi Degeri: ");
  Serial.println(nemDeger);                          // Nem değeri seri monitöre yazılması

  float Tempvalue = TempHum.getTempValue();        // Sıcaklık değeri alınması
  Serial.print("Sıcaklık = ");
  Serial.print(Tempvalue);
  
  float Humvalue = TempHum.getHumValue();          // Hava nemi değeri alınması
  Serial.print("C   Nem = %");
  Serial.println(Humvalue);

  Blynk.run();
  timer.run();

  if(millis() - zaman > period)
  {
    zaman = millis();
    Blynk.virtualWrite(V1, nemDeger);
    Blynk.virtualWrite(V2, Tempvalue);
  }
}
