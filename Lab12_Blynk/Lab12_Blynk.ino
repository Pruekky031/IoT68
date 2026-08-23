#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL60pDS3Loj"
#define BLYNK_TEMPLATE_NAME "2568"
#define BLYNK_AUTH_TOKEN "fphqFOmuW71ii9Btfm7IUiaO0ZN9GSoI"

#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include <Servo.h>
Servo myservo;
char ssid[] = "Pruek";
char pass[] = "pruekpruek";



int ActiveBlynkTime = 0;
int BlynkRunRound = 0;


#define OUTPUT_PIN 10



BLYNK_WRITE(V0) {
  int buttonState = param.asInt();  // 1 หรือ 0
  digitalWrite(OUTPUT_PIN, buttonState ? HIGH : LOW);
}


BLYNK_WRITE(V1) {
  int Servo_val = param.asInt();
  myservo.write(Servo_val);
}



//

void setup() {
  myservo.attach(3);
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  if (millis() - ActiveBlynkTime > 2000) {
    ActiveBlynkTime = millis();
    BlynkRunRound += 1;
    Blynk.run();
    Serial.print("Bynk Run: ");
    Serial.println(BlynkRunRound);
    Blynk.virtualWrite(V2, BlynkRunRound);
  }
}
