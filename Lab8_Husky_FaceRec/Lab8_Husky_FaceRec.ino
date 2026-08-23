/////////// import libary ///////////
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HUSKYLENS.h"
#include "Wire.h"
HUSKYLENS huskylens;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);

/////////// import libary ///////////

//////////// define Pin ////////////
#define BTN1 4
#define BUZZER 8
#define LED 2

//////////// define Pin ////////////

//////// initialize variable ////////

bool BTN1_Bool = true;
String State = "Standby";
bool BuzSCAN = true;
long Startnotfoundface;
bool notfoundface = true;

//////// initialize variable ////////

void setup() {
  Serial.begin(115200);
  Wire1.begin();
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
  pinMode(BTN1, INPUT);
  pinMode(LED, OUTPUT);
  // pinMode(BTN2, INPUT);
  Serial.println("Start///");
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);

  if (State == "Standby") {
    notfoundface == true;
    BuzSCAN = true;
    digitalWrite(LED, LOW);
    display.clearDisplay();
    display.println("System Ready!");
    display.println("Press Button to Scan");


    if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
      Serial.println("BTN1 press");
      State = "Scan";
      BTN1_Bool = false;
    } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
      Serial.println("BTN1 unpress");
      BTN1_Bool = true;
    }
  } else if (State == "Scan") {
    if (BuzSCAN == true) {
      display.clearDisplay();
      display.println("Scaning....");
      tone(BUZZER, 1000, 500);
      delay(1000);
      BuzSCAN = false;
    }

    if (huskylens.request()) {

      while (huskylens.available()) {
        notfoundface = true;
        HUSKYLENSResult result = huskylens.read();
        Serial.print("Color ID = ");
        Serial.print(result.ID);
        Serial.print(" | X = ");
        Serial.print(result.xCenter);
        Serial.print(" Y = ");
        Serial.println(result.yCenter);

        if (result.ID == 1) {
          Serial.print("1");
          display.clearDisplay();
          display.println("ACCESS GRANTED");
          display.display();
          Serial.println("found correct face");
          digitalWrite(LED, HIGH);
          tone(BUZZER, 1000, 400);
          delay(400);
          tone(BUZZER, 1500, 400);
          delay(400);
          tone(BUZZER, 2000, 400);
          delay(400);
          delay(2800);
          State = "Standby";
        } else if (result.ID == 0) {
          Serial.print("2");
          tone(BUZZER, 500, 2000);
          Serial.println("found uncorrect face");
          display.clearDisplay();
          display.setCursor(24, 0);
          display.println("ACCESS DENIED");
          display.drawLine(0, 0, 128, 64, SSD1306_WHITE);
          display.drawLine(128, 0, 0, 64, SSD1306_WHITE);
          display.display();
          for (int i = 0; i < 5; i++) {
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
            delay(100);
          }
          delay(1000);
          State = "Standby";
        }
      }

      Serial.print("3");
      if (notfoundface == true) {
        Startnotfoundface = millis();
        notfoundface = false;
      }
      
      if (millis() - Startnotfoundface > 5000) {
        Serial.println("TIMEOUT");
        tone(BUZZER, 4000, 1000);
        State = "Standby";
      }
    }
  }
  display.display();
}
