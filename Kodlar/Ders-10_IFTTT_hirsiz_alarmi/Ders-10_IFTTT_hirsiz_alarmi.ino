#include <WiFi.h>
#include <HTTPClient.h>
#include <Deneyap_HareketAlgilama.h>
#include <string.h>
Gesture Hareket;

const char* ssid = "Wifi adınız";
const char* password = "Wifi şifreniz";
const char* serverName = "http://maker.ifttt.com/trigger/hirsiz_alarmi/with/key/Api keyiniz";

unsigned long lastTime = 0;   
unsigned long timerDelay = 10000;   // Zamanlayıcı 10000 ms olarak ayarlarlandığı için alarm 10 saniyede bir tetiklenecektir.
uint16_t yakinlikDegeri;

void setup() {
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

  if (!Hareket.begin(0x32)) {
    delay(3000);
    Serial.println("I2C bağlantısı başarısız ");  // I2C bağlantısı başarısız olursa seri monitore yazdırılması
    while (1)
      ;
  }
}

void loop() {
  if ((millis() - lastTime) > timerDelay && hareketAlgila()) {
    if (WiFi.status() == WL_CONNECTED) {
      veriGonder();  
      
    } else {
      Serial.println("WiFi Baglantisi Koptu");
    }
    lastTime = millis();
  }
}

void veriGonder() {

  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //String veri = "value1=" + counterStr + "&value2=" + "0" + "&value3=" + "50";
  String httpRequestData = "value1=0&value2=&value3=0";
  int httpResponseCode = http.POST(httpRequestData);
  Serial.println("Gonderildi");
  http.end();
}

int hareketAlgila() {
  int durum = Hareket.readGesture();
  return (durum);
}