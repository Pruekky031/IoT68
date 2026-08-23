#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60pDS3Loj"
#define BLYNK_TEMPLATE_NAME "2568"
#define BLYNK_AUTH_TOKEN "fphqFOmuW71ii9Btfm7IUiaO0ZN9GSoI"

#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include <Wire.h>

BlynkTimer timer;

char ssid[] = "Pruek";
char pass[] = "pruekpruek";


BLYNK_WRITE(V0) {
  //param.asInt();
  //param.asFloat();
  //param.asStr();
}

void sendData() {
  Blynk.virtualWrite(V0, 0);
}

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();
}
