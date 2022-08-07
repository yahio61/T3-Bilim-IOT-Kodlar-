#include <WiFi.h>
#include <HTTPClient.h>
#include "lsm6dsm.h"
LSM6DSM IMU;

const char* ssid = "Ağınızın Adı";
const char* password = "Ağınızın Şifresi";
const char* serverName = "http://maker.ifttt.com/trigger/hava_durumu/with/key/Api Key'iniz";

unsigned long lastTime = 0;
// Zamanlayıcı 10000 ms olarak ayarlarlandığı için alarm 10 saniyede bir tetiklenebilir.
unsigned long timerDelay = 10000;
String sicaklik = "";
String nem = "";
void setup()
{
  Serial.begin(115200);
  pinMode(D8, INPUT);
  WiFi.begin(ssid, password);
  Serial.println("WiFi Baglaniyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi Agina Baglandi IP Addresi: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay && !digitalRead(D8))
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      havaDurumu();
      veriGonder();
    }
    else
    {
      Serial.println("WiFi Baglantisi Koptu");
    }
    lastTime = millis();
  }

}

void  veriGonder()
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "value1=" + sicaklik + "&value2=" + nem + "&value3=0";
  int httpResponseCode = http.POST(httpRequestData);
  Serial.println("Gonderildi");
  http.end();
}

void  havaDurumu(void)
{
  sicaklik = IMU.readTempC();
  nem = sicaklik;
}
