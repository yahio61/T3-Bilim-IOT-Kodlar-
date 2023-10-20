#include "WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Deneyap_SicaklikNemOlcer.h> // Deneyap_SicaklikNemOlcer kütüphane eklenmesi

#define WLAN_SSID       "***********"         // Baglanti kurulacak Wi-Fi agi adi
#define WLAN_PASS       "***********"         // Baglanti kurulacak Wi-Fi agi sifresi
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "***********"         // Adafruit IO kullanici ismi
#define AIO_KEY         "***********"         // Adafruit IO kullanici anahtari

#define FAN D5 // Fan pini

int sicaklik;
int nem;

TempHum TempHum;

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); // Adafruit client tanimlanmasi       

// Adafruit feedlerinin kodda tanimlanmasi
Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sicaklik"); 
Adafruit_MQTT_Publish hum = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/nem"); 
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fan"); 


void MQTT_connect() { // Adafruit sunucularina baglanti saglayan fonksiyon

  if (mqtt.connected()) 
  {
    return;
  }
  
  Serial.print("Bulut sistemine baglaniliyor... ");

  int8_t ret;
  uint8_t count = 3;
  while ((ret = mqtt.connect()) != 0) 
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("3 saniye icinde tekrar baglanti denemesi gerceklesecek...");
    mqtt.disconnect();
    delay(3000); 
    count = count - 1 ;

    if (count == 0) 
    {
      esp_restart();
    }
  }
  
  Serial.println("Bulut sistemine baglanildi!");
}


void setup() {
  Serial.begin(115200);
  delay(10);

  if(!TempHum.begin(0x70)){                        // begin(slaveAdress) fonksiyonu ile I2C cihazlarinin haberlesmesinin baslatilmasi
      delay(300);
      Serial.println("I2C bağlantısı başarısız ");  // I2C baglantisinin basarisiz olmasi durumunda terminale yazdirilmasi
   }

  //WIFI BAGLANTISI SAGLAMA
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.println("WiFi Baglaniyor");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi Agina Baglandi IP Addresi: ");
  Serial.println(WiFi.localIP());

  // Gerekli pinlerin tanimlamalari
  pinMode(LEDR, OUTPUT);
  pinMode(FAN, OUTPUT);

  digitalWrite(FAN, LOW);

  mqtt.subscribe(&fan); // Adafruit'ten okumak istedigimiz veriya baglanti saglanmasi
}

void loop() {
  MQTT_connect();
  
  //ADAFRUIT UZERINDEN TUS HAREKETI DINLEME
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &fan) {

      Serial.print(F("Got: "));
      Serial.println((char *)fan.lastread);

       if (!strcmp((char*) fan.lastread, "ON"))
      {
        digitalWrite(FAN, HIGH);
      }
      else
      {
        digitalWrite(FAN, LOW);
      }
    }
  }

  // Nem ve sicaklik sensorunden verilerin alinmasi ve degiskenlere yazilmasi
  sicaklik = TempHum.getTempValue();
  nem = TempHum.getHumValue();

  // Verilerin sunucuya yollanmasi
  temp.publish(sicaklik);
  hum.publish(nem);

  // Baglantiyi kaybetmemek adina sunucuya devamli ping gonderilmesi
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}
