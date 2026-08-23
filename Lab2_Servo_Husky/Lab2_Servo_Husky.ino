#include "HUSKYLENS.h"
#include "Wire.h"
#include <Servo.h>

Servo myservo;
HUSKYLENS huskylens;

int ServoRad;

void setup() {
  Serial.begin(115200);
  Serial.println("Start///");
  Wire1.begin();
  myservo.attach(9);
  if (!huskylens.begin(Wire1)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
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
      ServoRad = map(result.xCenter, 0, 320, 180, 0);
      Serial.print("ServoRad: ");
      Serial.println(ServoRad);
      myservo.write(ServoRad);
    }
  }
}