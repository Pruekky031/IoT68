#include "HUSKYLENS.h"
#include "Wire.h"
#include <Servo.h>
HUSKYLENS huskylens;
Servo myservo;
#define LEDRED 3
#define LEDGREEN 2

#define servo 9
#define buzzer 7
#define BTN 5
bool BTNBools = true;
void setup() {
  Serial.begin(115200);
  Serial.println("Start//");
  pinMode(BTN, INPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  myservo.attach(9);
  Wire.begin();
  if (!huskylens.begin(Wire)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
  myservo.write(90);
}

void loop() {
  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      Serial.print("Color ID = ");
      Serial.print(result.ID);
      Serial.print(" | X = ");
      Serial.print(result.xCenter);
      Serial.print(" Y = ");
      Serial.println(result.yCenter);

      if (digitalRead(BTN) == 0 && BTNBools) {
        digitalWrite(LEDGREEN, HIGH);
        digitalWrite(LEDRED, LOW);
        
        if (result.ID == 1) {
          tone(buzzer, 3000, 300);
          delay(300);
          tone(buzzer, 3500, 300);
          delay(300);
          tone(buzzer, 4000, 300);
          delay(300);
          myservo.write(180);

        } else if (result.ID == 2) {
          digitalWrite(LEDRED, HIGH);
          digitalWrite(LEDGREEN, LOW);
          tone(buzzer, 1000, 300);
          delay(300);
          tone(buzzer, 500, 300);
          delay(300);
          tone(buzzer, 1000, 300);
          myservo.write(0);
        }
        BTNBools = false;
      } else if (digitalRead(BTN) == 1 && !BTNBools) {
        BTNBools = true;
      }
    }
  }
}
