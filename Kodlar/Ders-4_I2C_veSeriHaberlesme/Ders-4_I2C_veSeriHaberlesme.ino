#include <Deneyap_MesafeOlcerIsikAlgilayici.h> 
#include <Deneyap_SicaklikNemOlcer.h>

ProximityAL Yakinlik; 
TempHum TempHum; 

uint16_t yakinlikDegeri;
float Tempvalue;

void setup() {
    Serial.begin(115200);
    Yakinlik.begin(0x23);  
    TempHum.begin(0x70);
}

void loop() {
    yakinlikDegeri = Yakinlik.getPSvalue();
    Tempvalue = TempHum.getTempValue();
    Serial.print("Yakınlık Değeri = ");
    Serial.print(yakinlikDegeri);
    Serial.print("    Sıcaklık = ");
    Serial.print(Tempvalue);
    
    delay(500);
}
