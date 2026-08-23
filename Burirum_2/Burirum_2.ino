#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60pDS3Loj"
#define BLYNK_TEMPLATE_NAME "2568"
#define BLYNK_AUTH_TOKEN "fphqFOmuW71ii9Btfm7IUiaO0ZN9GSoI"


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <Wire.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include "HUSKYLENS.h"
#include "Wire.h"
HUSKYLENS huskylens;

BlynkTimer timer;

char ssid[] = "Pruek";
char pass[] = "pruek123";
unsigned long lastRead = 0;
float Temp = 0;
int Hum = 0;
ArduinoLEDMatrix matrix;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


#define BTN1 3
#define BTN2 4
#define Buzz 2

#define LEDRED 9
#define LEDGREEN 10
#define LEDYELLOW 11
#define POT A1
bool BTN1_Bool = true;
bool BTN2_Bool = true;
bool DoubleBTN = true;
int BlynkBTN = 0;

bool Select_Mode = false;
int Mode = 0;
int R = 0;
int G = 0;
int Y = 0;
int SumBall = 0;
bool boolsRGY = true;

unsigned long DetectMillis = 0;

BLYNK_WRITE(V4) {
  BlynkBTN = param.asInt();
  if (BlynkBTN == 1) {
    R = 0;
    G = 0;
    Y = 0;
  }
  //param.asFloat();
  //param.asStr();
}
void sendData() {
  Blynk.virtualWrite(V0, R);
  Blynk.virtualWrite(V1, G);
  Blynk.virtualWrite(V2, Y);
  Blynk.virtualWrite(V3, SumBall);
}


void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDYELLOW, OUTPUT);


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendData);
  matrix.begin();
  matrix.beginDraw();
  matrix.textScrollSpeed(60);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);


  Wire.begin();
  Wire1.begin();
  if (!huskylens.begin(Wire1)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
}

bool locked = false;           // ล็อกว่าลูกนี้นับแล้ว
int lastID = 0;                // จำ ID ล่าสุด
unsigned long lostMillis = 0;  // เวลาที่เริ่มไม่เห็นลูก

void loop() {

  display.clearDisplay();


  if (!Select_Mode) {


    if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
      Serial.println("BTN1 press");
      BTN1_Bool = false;
    } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
      Mode = !Mode;
      Serial.print("Mode: ");
      Serial.println(Mode);
      Serial.println("BTN1 unpress");
      BTN1_Bool = true;
    }

    if (digitalRead(BTN2) == 0 && BTN2_Bool == true) {
      Serial.println("BTN2 press");
      BTN2_Bool = false;
    } else if (digitalRead(BTN2) == 1 && BTN2_Bool == false) {
      Select_Mode = true;
      if (Mode == 1) {

        display.setCursor(35, 0);
        display.print("WiFi Status");
        display.setCursor(0, 15);
        display.print("WiFi....OK");
        display.setCursor(0, 30);
        display.print("Blynk....OK");
        display.display();
        delay(1000);
      }
      Serial.println("BTN2 unpress");
      BTN2_Bool = true;
    }
    display.setCursor(35, 0);
    display.print("Color Ball");

    display.setCursor(12, 15);
    display.print("Offline");
    display.setCursor(12, 30);
    display.print("Online");

    display.fillTriangle(
      3 + 4, Mode * 15 + 2 + 15,
      3, Mode * 15 + 15,
      3, Mode * 15 + 4 + 15,
      WHITE);
  }

  if (Select_Mode) {
    if (Mode == 1) {
      Blynk.run();
      timer.run();
    }
    SumBall = R + G + Y;
    if (SumBall < 10) {
      if (huskylens.request()) {

        bool found = false;

        while (huskylens.available()) {
          HUSKYLENSResult result = huskylens.read();
          found = true;

          if (!locked) {

            // ปิดทุก LED ก่อน
            digitalWrite(LEDRED, LOW);
            digitalWrite(LEDGREEN, LOW);
            digitalWrite(LEDYELLOW, LOW);

            if (result.ID == 1) {
              R++;
              Serial.println("Red");
              digitalWrite(LEDRED, HIGH);
            } else if (result.ID == 2) {
              G++;
              Serial.println("Green");
              digitalWrite(LEDGREEN, HIGH);
            } else if (result.ID == 3) {
              Y++;
              Serial.println("Yellow");
              digitalWrite(LEDYELLOW, HIGH);
            }

            lastID = result.ID;
            locked = true;
          }
        }

        // ถ้าไม่เจอลูกเลย
        if (!found) {
          if (millis() - lostMillis > 200) {
            locked = false;
            lastID = 0;

            // ดับ LED เมื่อไม่มีลูก
            digitalWrite(LEDRED, LOW);
            digitalWrite(LEDGREEN, LOW);
            digitalWrite(LEDYELLOW, LOW);
          }
        } else {
          lostMillis = millis();
        }
      }


      display.setCursor(35, 0);
      display.print("Color Ball");

      display.setCursor(3, 12);
      display.print("R = ");
      display.print(R);

      display.setCursor(3, 28);
      display.print("G = ");
      display.print(G);

      display.setCursor(3, 43);
      display.print("Y = ");
      display.print(Y);

      display.setCursor(3, 55);
      display.print("Total = ");
      display.print(SumBall);
    } else {

      digitalWrite(LEDRED, LOW);
      digitalWrite(LEDGREEN, LOW);
      digitalWrite(LEDYELLOW, LOW);


      display.setCursor(35, 0);
      display.print("Total 10");
      if (R > G && R > Y) {
        display.setCursor(3, 15);
        display.print("R Most = ");
        display.print(R);
      } else if (G > R && G > Y) {
        display.setCursor(3, 15);
        display.print("G Most = ");
        display.print(G);
      } else if (Y > R && Y > G) {
        display.setCursor(3, 15);
        display.print("Y Most = ");
        display.print(Y);
      } else {
        display.setCursor(3, 15);
        display.print("Equal");
      }
      display.setCursor(13, 45);
      display.print("sw1 for reset");
      display.display();

      if (BlynkBTN == 1 || digitalRead(BTN1) == 0) {
        R = 0;
        G = 0;
        Y = 0;
      }
    }
  }
  display.display();
}
