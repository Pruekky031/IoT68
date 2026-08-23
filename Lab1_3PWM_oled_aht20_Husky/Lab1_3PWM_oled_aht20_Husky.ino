#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HUSKYLENS.h"
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
HUSKYLENS huskylens;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LED_Green 2
#define LED_Yellow 3
#define LED_Red 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);


int index[5] = { 1, 2, 3, 4, 5 };
String name[5] = { "Green", "Red", "Black", "Blue", "Yellow" };

int Sum_obj = 0;
String str = "";
String col_name;


void setup() {
  Serial.begin(115200);
  Serial.println("start");
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  Wire1.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }

  if (!huskylens.begin(Wire1)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
  Wire.begin();  // ใช้บัสหลัก Bus0 (ขา SDA=20, SCL=21)

  if (!aht.begin(&Wire)) {  // ← แก้ตรงนี้
    Serial.println("Not Found AHT20 !");
    while (1) delay(10);
  }

  Serial.println("AHT20 Ready!");


  display.setTextColor(SSD1306_WHITE);
}
void loop() {
  display.clearDisplay();
  display.setTextSize(1);

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);


  str = "";
  col_name = "";
  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      str = str + result.ID;
      if (result.ID == 1) {
        col_name += "GR";
        col_name += " / ";
      }
      if (result.ID == 2) {
        col_name += "RD";
        col_name += " / ";
      }
      if (result.ID == 3) {
        col_name += "BK";
        col_name += " / ";
      }
      if (result.ID == 4) {
        col_name += "BE";
        col_name += " / ";
      }
      if (result.ID == 5) {
        col_name += "YL";
      }
    }
    Sum_obj = str.length();

    if (str.indexOf("1") != -1) {
      digitalWrite(LED_Green, HIGH);
    } else {
      digitalWrite(LED_Green, LOW);
    }
    if (str.indexOf("5") != -1) {
      digitalWrite(LED_Yellow, HIGH);
    } else {
      digitalWrite(LED_Yellow, LOW);
    }
    if (str.indexOf("2") != -1) {
      digitalWrite(LED_Red, HIGH);
    } else {
      digitalWrite(LED_Red, LOW);
    }
  }


  display.setCursor(0, 10);
  display.print("obj Count: ");
  display.println(Sum_obj);

  display.print("Temp : ");
  display.print(temp.temperature);
  display.println(" C");

  display.print("Humid: ");
  display.print(humidity.relative_humidity);
  display.println(" %");
  display.print("Found: ");
  display.println(col_name);
  display.display();
  delay(2000);
}