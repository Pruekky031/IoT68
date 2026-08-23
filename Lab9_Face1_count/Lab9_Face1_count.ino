/////////// import libary ///////////
#include "HUSKYLENS.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;
HUSKYLENS huskylens;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);
/////////// import libary ///////////

//////////// define Pin ////////////
#define BTN1 4
#define BUZZER 8
#define LED 2

uint8_t Noneframe[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
uint8_t Hearthframe[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
};
uint8_t Xframe[8][12] = {
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }
};
//////////// define Pin ////////////

//////// initialize variable ////////
bool BTN1_Bool = true;
bool Result1_Bool = true;
int Counter = 0;
unsigned long BTN_Time;
String Mode = "Normal";
//////// initialize variable ////////

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire1.begin();
  matrix.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }
  if (!huskylens.begin(Wire)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }


  pinMode(LED, OUTPUT);
  Serial.println("Start///");
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);



  if (huskylens.request()) {
    if (digitalRead(BTN1) == 1 && Mode == "Normal") {

      display.println("System Ready! ");
      display.println("Scaning......");



      while (huskylens.available()) {
        HUSKYLENSResult result = huskylens.read();
        if (result.ID == 1) {
          Counter += 1;
          display.print("Welcome ID1 ");
          
          display.display();
          matrix.renderBitmap(Hearthframe, 8, 12);
          digitalWrite(LED, HIGH);
          tone(BUZZER, 2000, 300);
          delay(300);
          tone(BUZZER, 2000, 300);
          delay(300);
          digitalWrite(LED, LOW);
          matrix.renderBitmap(Noneframe, 8, 12);
          delay(1000);
        } else if (result.ID == 0) {
          matrix.renderBitmap(Xframe, 8, 12);
          display.println("WARNING!");
          display.println("Intruder Detected");
          display.display();
          tone(BUZZER, 500, 2000);
          delay(2000);
          matrix.renderBitmap(Noneframe, 8, 12);
        }
      }
    } else if (Mode == "display") {
      display.print("Welcome ID1 Count: ");
      display.println(Counter);
    }




    if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
      Serial.println("BTN1 press");
      BTN_Time = millis();
      BTN1_Bool = false;
    } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
      Serial.println("BTN1 unpress");
      if (millis() - BTN_Time > 2000) {
        Serial.println("Reset");
        Counter = 0;
      } else {
        if (Mode == "Normal") {
          Mode = "display";
        } else if (Mode == "display") {
          Mode = "Normal";
        }
      }
      BTN1_Bool = true;
    }
  }
  display.display();
}
