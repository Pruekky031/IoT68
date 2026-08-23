#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HUSKYLENS.h"
#include "Wire.h"
HUSKYLENS huskylens;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);
#define BTN1 4
#define SOUND_PIN 8
#define LED 2

bool BTN1_Bool = true;
int RedCount = 0;
int BlueCount = 0;
bool found = false;
unsigned long Time;
char oledMode = 'S';
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BTN1, INPUT);

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

  Serial.println("start///");
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



      if (result.ID == 1) {
        RedCount += 1;
        digitalWrite(LED, HIGH);
        tone(SOUND_PIN, 1000, 500);
        delay(500);
        digitalWrite(LED, LOW);
        delay(500);
      } else if (result.ID == 2) {
        BlueCount += 1;
        digitalWrite(LED, HIGH);
        tone(SOUND_PIN, 1200, 300);
        delay(300);
        digitalWrite(LED, LOW);
        delay(300);
        tone(SOUND_PIN, 1200, 300);
        digitalWrite(LED, HIGH);
        delay(300);
        digitalWrite(LED, LOW);
        delay(300);
        digitalWrite(LED, HIGH);
        delay(300);
        digitalWrite(LED, LOW);
      } else {
      }
      found = true;
    }
  }

  if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
    Serial.println("BTN1 press");
    if (oledMode == 'T') {
      oledMode = 'S';
    } else if (oledMode == 'S') {
      oledMode = 'T';
    }
    Time = millis();
    BTN1_Bool = false;
  } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
    noTone(SOUND_PIN);
    Serial.println("BTN1 unpress");
    BTN1_Bool = true;
  }




  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);

  if (oledMode == 'S') {
    display.print("    Color Sorting");
    display.setCursor(0, 30);
    display.print("    Ready to Scan");
    display.setCursor(0, 40);
    display.print("Red: ");
    display.print(RedCount);

    display.print("       Blue: ");
    display.print(BlueCount);

    if (found == true) {

    } else {
      display.setCursor(30, 50);
      display.print("waiting...");
    }
  } else if (oledMode == 'T') {
    if (digitalRead(BTN1) == 0) {
      if (millis() - Time > 3000) {
        tone(SOUND_PIN, 1200);

        display.setCursor(0, 30);
        display.print("    Data Clearad");
        RedCount = 0;
        BlueCount = 0;
      }
    }
    display.setCursor(0, 10);
    display.print("    Totel: ");
    display.print(RedCount + BlueCount);
  }


  display.display();
  found = false;
}
