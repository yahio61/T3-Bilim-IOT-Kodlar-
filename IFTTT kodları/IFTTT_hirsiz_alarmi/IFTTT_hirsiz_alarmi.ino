#include <WiFi.h>
#include <HTTPClient.h>
#include <Deneyap_MesafeOlcerIsikAlgilayici.h>                 // Deneyap_MesafeOlcerIsikAlgilayici.h kütüphanesi eklenmesi

ProximityAL Yakinlik; 

const char* ssid = "Ağınızın Adı";
const char* password = "Ağınızın Şifresi";
const char* serverName = "http://maker.ifttt.com/trigger/hirsiz_alarmi/with/key/Api Key'iniz";

unsigned long lastTime = 0;
// Zamanlayıcı 10000 ms olarak ayarlarlandığı için alarm 10 saniyede bir tetiklenebilir.
unsigned long timerDelay = 10000;
uint16_t yakinlikDegeri;
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
  Yakinlik.begin(0x23);
}

void loop() {
  if ((millis() - lastTime) > timerDelay && uzaklikOlc())
  {
    if (WiFi.status() == WL_CONNECTED)
    {
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
  //"value1=" + "12" + "&value2=" + "43" + "&value3=" + "54"
  String httpRequestData = "value1=0&value2=0&value3=0";
  int httpResponseCode = http.POST(httpRequestData);
  Serial.println("Gonderildi");
  http.end();
}

int  uzaklikOlc()
{
  int durum = 0;
  yakinlikDegeri = Yakinlik.getPSvalue();
  Serial.println(yakinlikDegeri);
  delay(100);
  if (yakinlikDegeri >1500)
  {
    durum = 1;
  }
  return (durum);
}
