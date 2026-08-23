#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include "Arduino_LED_Matrix.h"
#include "HUSKYLENS.h"
#include "Wire.h"

HUSKYLENS huskylens;
Servo myservo;
ArduinoLEDMatrix matrix;
#define BTN1 4

#define SOUND_PIN 8
#define LED 2
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);

bool BTN1_Bool = true;
bool systempause = false;


uint8_t DownArrow[8][12] = {
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
};
uint8_t X[8][12] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }
};
uint8_t clear[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  if (!huskylens.begin(Wire)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }
  myservo.attach(9);
  matrix.begin();
  pinMode(LED, OUTPUT);
  myservo.write(0);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Parcel System");
  display.println("Ready");
  display.display();

  matrix.renderBitmap(DownArrow, 8, 12);
  Serial.println("Start///");
  delay(2000);
}
void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);

  if (huskylens.request()) {
    if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
      Serial.println("BTN1 press");
      if (systempause == true) {
        systempause = false;
      } else if (systempause == false) {
        systempause = true;
      }
      BTN1_Bool = false;
    } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
      Serial.println("BTN1 unpress");
      BTN1_Bool = true;
    }

    while (huskylens.available() && !systempause) {
      HUSKYLENSResult result = huskylens.read();
      Serial.print("Color ID = ");
      Serial.print(result.ID);
      Serial.print(" | X = ");
      Serial.print(result.xCenter);
      Serial.print(" Y = ");
      Serial.println(result.yCenter);

      if (result.ID == 1) {
        matrix.renderBitmap(clear, 8, 12);

        myservo.write(90);
        display.clearDisplay();
        display.println("Bottle");
        display.print("X: ");
        display.println(result.xCenter);
        display.display();
        tone(SOUND_PIN, 1000, 500);
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        delay(2000);
      } else if (result.ID == 2) {
        matrix.renderBitmap(clear, 8, 12);

        myservo.write(180);
        display.clearDisplay();
        display.println("Parcel ID: 2");
        display.display();
        tone(SOUND_PIN, 2000, 500);
        delay(1000);
        tone(SOUND_PIN, 2000, 500);
        delay(1000);
        delay(1000);
      } else {
        Serial.println("X");
        matrix.renderBitmap(X, 8, 12);
        myservo.write(0);
      }
    }
    if (systempause == true) {
      display.clearDisplay();
      display.println("System Paused");
      display.display();
      Serial.println("pause");
      matrix.renderBitmap(clear, 8, 12);

    } else {
      display.clearDisplay();
      display.display();
      Serial.println("X");
      matrix.renderBitmap(X, 8, 12);
      myservo.write(0);
    }
  }
}
