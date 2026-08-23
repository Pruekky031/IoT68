#include "HUSKYLENS.h"
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

HUSKYLENS huskylens;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);


#define REDLED 7
#define BTN 2
#define BUZZER 3



void setup() {
  Serial.begin(115200);
  Serial.println("Start////");


  pinMode(BTN, INPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(BUZZER, OUTPUT);


  Wire1.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not connected!");

    for (;;)
      ;
  }

  if (!huskylens.begin(Wire1)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
}
int oledX = 0;
int oledY = 0;


int hitboxX = 55;
int hitboxW = 20;
int hitboxH = 50;

int hitboxUPY = -30;
int hitboxDOWNY = 42;
bool GameEND = false;
bool scrollbool = true;
void loop() {
  //วาดสี่เหลี่ยม
  if (!GameEND) {
    if (hitboxX + hitboxW < 128 && scrollbool) {
      hitboxX += 1;
      
  
    } else {
      scrollbool = false;
    }
    if (hitboxX > 0 && !scrollbool) {
      hitboxX -= 1;
      
    } else {
      scrollbool = true;
    }
    display.clearDisplay();
    display.drawRect(hitboxX, hitboxUPY, hitboxW, hitboxH, SSD1306_WHITE);
    display.fillRect(hitboxX, hitboxUPY, hitboxW, hitboxH, SSD1306_WHITE);

    display.drawRect(hitboxX, hitboxDOWNY, hitboxW, hitboxH, SSD1306_WHITE);
    display.fillRect(hitboxX, hitboxDOWNY, hitboxW, hitboxH, SSD1306_WHITE);

    display.setCursor(oledX, oledY);
    display.print(".");


  } else {
    if (digitalRead(BTN) == 0) {
      noTone(BUZZER);
      analogWrite(REDLED, 0);
      Serial.println("EIEI");
      GameEND = false;
    }
    display.clearDisplay();
  }



  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();

      oledX = map(result.xCenter, 0, 320, 0, 128);
      oledY = map(result.yCenter, 0, 240, 0, 64);

      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(oledX, oledY);
      display.print(".");


      if (!GameEND) {
        if (oledX >= hitboxX && oledX <= hitboxX + hitboxW) {

          if (oledY > 30) {
            GameEND = true;
          }
          if (oledY < 10) {
            GameEND = true;
          }
        } else {
          analogWrite(REDLED, 0);
        }
      } else if (GameEND) {
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("END");
        tone(BUZZER, 1000);
        analogWrite(REDLED, 255);
      }
    }
  }
  display.display();
}
