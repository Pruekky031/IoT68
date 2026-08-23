#include "HUSKYLENS.h"
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

HUSKYLENS huskylens;
ArduinoLEDMatrix matrix;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);

void setup() {
  Serial.begin(115200);
  Wire1.begin();
  matrix.begin();
  matrix.beginDraw();
  matrix.clear();
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
int MatrixX = 0;
int MatrixY = 0;
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

        oledX = map(result.xCenter, 0, 320, 0, 128);
        oledY = map(result.yCenter, 0, 240, 0, 64);

      MatrixY = map(result.xCenter, 0, 320, 0, 8);
      MatrixX = map(result.yCenter, 0, 240, 0, 12);

      Serial.print("  OLED | X = ");
      Serial.print(oledX);
      Serial.print("  OLED Y = ");
      Serial.print(oledY);
      Serial.print("  Matrix | X = ");
      Serial.print(MatrixX);
      Serial.print("  Matrix | Y = ");
      Serial.println(MatrixY);

      display.clearDisplay();
      matrix.clear();

      matrix.stroke(255, 0, 0);
      matrix.point(MatrixX, MatrixY);
      matrix.endDraw();

      display.setTextColor(SSD1306_WHITE);
      display.setCursor(oledX, oledY);
      display.print(".");
      display.display();
    }
  }
}
