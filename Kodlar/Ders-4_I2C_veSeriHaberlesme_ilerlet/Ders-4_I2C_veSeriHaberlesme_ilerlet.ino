#include <Deneyap_SicaklikNemOlcer.h>

TempHum TempHum; 

float Tempvalue;

void setup() {
    Serial.begin(115200); 
    TempHum.begin(5);
}

void loop() {
    Tempvalue = TempHum.getTempValue();
    Serial.print("    Sıcaklık = ");
    Serial.print(Tempvalue);
    
    delay(500);
}
