#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <Deneyap_SicaklikNemOlcer.h>

TempHum TempHum;

float Temperature;
float Humidity;

const char* ssid = "WİFİ adı"; 
const char* password = "Wifi Şifre";

WiFiServer server(80);

String header;

void setup() {
Serial.begin(115200);

if(!TempHum.begin(0x70)){                        
      delay(3000);
      Serial.println("Sicaklik sensoru I2C bağlantısı başarısız ");  
   }

Serial.print("Aga baglandi");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("Wifiye Baglandi.");
Serial.println("Deneyap Kart IP address : ");
Serial.println(WiFi.localIP());
server.begin();
Serial.println("Server basladi");

}

void loop()
{
Temperature = TempHum.getTempValue(); 
Humidity = TempHum.getHumValue(); 
WiFiClient client = server.available();

if (client) 
{ 
Serial.println("Web Client baglandi ");
String request = client.readStringUntil('\r'); 
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("</style></head><body><h1>Deneyap Kart Web Server Sensor degerlerini okuyor</h1>");
client.println("<h2>DHT11/DHT22</h2>");
client.println("<h2>Deneyap Kart</h2>");
client.println("<table><tr><th>Sicaklik</th><th>VALUE</th></tr>");
client.println("<tr><td>Sicaklik Deger</td><td><span class=\"sensor\">");
client.println(TempHum.getTempValue());
client.println(" *C</span></td></tr>"); 
client.println("<tr><td>Sicaklik Fahrenheit</td><td><span class=\"sensor\">");
client.println(1.8 * TempHum.getTempValue() + 32);
client.println(" *F</span></td></tr>"); 
client.println("<tr><td>Nem Degeri</td><td><span class=\"sensor\">");
client.println(TempHum.getHumValue());
client.println(" %</span></td></tr>"); 
client.println("</body></html>"); 
client.stop();
client.println();
Serial.println("Baglanti sonlandi.");
Serial.println("");
}
}