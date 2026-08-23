#include "HUSKYLENS.h"
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

HUSKYLENS huskylens;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);


#define REDLED 9
#define BUZZER 10



void setup() {
  Serial.begin(115200);
  Serial.println("Start////");
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
int oledXStart = 0;
int oledYStart = 0;
int oledXEnd = 0;
int oledYEnd = 0;

String TEXT = "Target";
int TextLen = TEXT.length();

int textpxlenX = TextLen * 6 - 1;
int textpxlenY = 7;

int xstart;
int xend;
int ystart;
int yend;

int textstartOLEDX = 0;
int textstartOLEDY = 0;

bool scrollbool = true;

void loop() {

  if (textstartOLEDX + textpxlenX < 128 && scrollbool) {
    textstartOLEDX += 1;
    textstartOLEDY = 0;
  } else {
    scrollbool = false;
  }
  if (textstartOLEDX > 0 && !scrollbool) {
    textstartOLEDX -= 1;
    textstartOLEDY = 0;
  } else {
    scrollbool = true;
  }

  xstart = textstartOLEDX;
  xend = textstartOLEDX + textpxlenX;
  ystart = textstartOLEDY;
  yend = 6;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(textstartOLEDX, textstartOLEDY);
  display.println(TEXT);
  display.drawLine(oledXStart, oledYStart, oledXEnd, oledYEnd, SSD1306_WHITE);
  display.display();
  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();

      oledXStart = map(result.xOrigin, 0, 320, 0, 128);
      oledYStart = map(result.yOrigin, 0, 240, 0, 64);

      oledXEnd = map(result.xTarget, 0, 320, 0, 128);
      oledYEnd = map(result.yTarget, 0, 240, 0, 64);

      Serial.print("  OLED Start | X = ");
      Serial.print(oledXStart);
      Serial.print("  OLED Start | Y = ");
      Serial.print(oledYStart);
      Serial.print("  OLED End | X = ");
      Serial.print(oledXEnd);
      Serial.print("  OLED End | Y = ");
      Serial.println(oledYEnd);




      display.drawLine(oledXStart, oledYStart, oledXEnd, oledYEnd, SSD1306_WHITE);
      display.display();



      if (oledXEnd >= xstart && oledXEnd <= xend) {
        if (oledYEnd >= ystart && oledYEnd <= yend) {
          digitalWrite(REDLED, HIGH);
          tone(BUZZER, 500);
        }
      } else {
        digitalWrite(REDLED, LOW);
        noTone(BUZZER);
      }
    }
  } else {
    display.clearDisplay();
    display.display();
  }
}