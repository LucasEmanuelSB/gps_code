#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
HTTPClient http;

const int LED = 2;

double latitude = 0.0;
double longitude = 0.0;

#define TASK_SERIAL_RATE 1000 // ms
uint32_t nextSerialTaskTs = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  if (nextSerialTaskTs < millis()) {
    Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
    Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
    Serial.print("Sats=");  Serial.println(gps.satellites.value());
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    if(latitude != 0.0 && longitude != 0.0){  
      postDataToServer();
      digitalWrite(LED, LOW);
      delay(1000);
      digitalWrite(LED, HIGH);  
    }
    
    nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
  }
}
