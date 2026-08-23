#include "HUSKYLENS.h"
#include "Wire.h"
#include <Servo.h>


Servo myservo;
HUSKYLENS huskylens;

#define SOUND_PIN 9
#define LED_Green 2
#define LED_Yellow 3
#define LED_Red 4
int ServoRad;
String str;
void setup() {
  Serial.begin(115200);
  Serial.println("Start///");
  Wire1.begin();
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  // myservo.attach(9);
  if (!huskylens.begin(Wire1)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
}
void loop() {
  str = "";
  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      Serial.print("Color ID = ");
      Serial.print(result.ID);
      Serial.print(" | X = ");
      Serial.print(result.xCenter);
      Serial.print(" Y = ");
      Serial.println(result.yCenter);

      str = str + result.ID;
    }
  }
  Serial.println(str);
  if (str == "1") {
    tone(SOUND_PIN, 2000, 100);
    tone(SOUND_PIN, 3000, 100);
    delay(200);
    digitalWrite(LED_Red, HIGH);
  } else if (str == "2") {
    digitalWrite(LED_Green, HIGH);
  } else if (str == "12") {
    digitalWrite(LED_Green, HIGH);
    digitalWrite(LED_Yellow, HIGH);
    digitalWrite(LED_Red, HIGH);
  } else {
    digitalWrite(LED_Green, LOW);
    digitalWrite(LED_Yellow, LOW);
    digitalWrite(LED_Red, LOW);
    noTone(SOUND_PIN);
  }
}