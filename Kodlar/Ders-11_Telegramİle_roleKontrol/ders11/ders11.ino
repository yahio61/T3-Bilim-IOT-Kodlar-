#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Deneyap_Role.h>

// Wifi bilgileri
const char* ssid = "******************";
const char* password = "******************";

//Bot Token
#define BOTtoken "******************"  

//Chat ID
#define CHAT_ID "******************"


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
Relay Role;


int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

bool relayState = LOW;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Onyasız kullanıcı", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Merhaba, " + from_name + ".\n";
      welcome += "Roleyi kontrol etmek için aşağıdaki komutları kulanın.\n\n";
      welcome += "/role_aktif :Roleyi açmak için \n";
      welcome += "/role_pasif :Roleyi kapatmak için \n";
      welcome += "/durum :Rolenin durumunu öğrenmek için \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/role_aktif") {
      bot.sendMessage(chat_id, "Role acildi", "");
      relayState = HIGH;
      Role.RelayDrive(relayState);
    }
    
    if (text == "/role_pasif") {
      bot.sendMessage(chat_id, "Role Kapatildi", "");
      relayState = LOW;
      Role.RelayDrive(relayState);
    }
    
    if (text == "/durum") {
      if (relayState){
        bot.sendMessage(chat_id, "Role acik", "");
      }
      else{
        bot.sendMessage(chat_id, "Role kapali", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  if(!Role.begin(0x0C)){             
      delay(1000);
      Serial.println("Role I2C bağlantısı başarısız "); 
   }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}