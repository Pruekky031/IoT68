#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include "Arduino_LED_Matrix.h"
#include "HUSKYLENS.h"
#include "Wire.h"

HUSKYLENS huskylens;
Adafruit_AHTX0 aht;
Servo myservo;
ArduinoLEDMatrix matrix;




uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
};
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define LED 10
#define Buzzer 2
#define BTN1 3
#define BTN2 4
#define Servo 9
#define POT A1

unsigned long lastRead = 0;
float TEM = 0;
float HUM = 0;
bool BTN1_Bool = true;
bool BTN2_Bool = true;
bool LED_bool = true;
int ProtenVal;
int ProtenValMap;
int Conut = 0;
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  myservo.attach(Servo);

  matrix.begin();
  Wire.begin();

  if (!huskylens.begin(Wire)) {
    Serial.println("HuskyLens not connected!");
    while (1)
      ;
  }
  if (!aht.begin(&Wire)) {
    Serial.println("Not Found AHT20 !");
    while (1) delay(10);
  }
  Serial.println("AHT20 Ready!");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }
}

void loop() {
  if (millis() - lastRead >= 2000) {
    lastRead = millis();

    TEM = Get_Temp_Hum('T');
    HUM = Get_Temp_Hum('H');
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Count: ");
  display.println(Conut);
  display.print("Temp: ");
  display.println(TEM);
  display.print("Humu: ");
  display.println(HUM);

  matrix.renderBitmap(frame, 8, 12);


  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      Serial.print("Color ID = ");
      Serial.print(result.ID);
      Serial.print(" | X = ");
      Serial.print(result.xCenter);
      Serial.print(" Y = ");
      Serial.println(result.yCenter);


      display.print("ID: ");
      display.print(result.ID);
      display.print(" | X = ");
      display.println(result.xCenter);
      display.print(" Y = ");
      display.println(result.yCenter);
      display.display();
    }
  }

  if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
    Serial.println("BTN1 press");

    tone(Buzzer, 3000, 100);
    if (LED_bool) {
      LED_bool = false;
    } else {
      LED_bool = true;
    }


    BTN1_Bool = false;
  } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
    Serial.println("BTN1 unpress");
    tone(Buzzer, 3000, 100);

    BTN1_Bool = true;
  }

  if (digitalRead(BTN2) == 0 && BTN2_Bool == true) {
    Serial.println("BTN2 press");
    tone(Buzzer, 3500, 100);
    Conut += 1;
    BTN2_Bool = false;
  } else if (digitalRead(BTN2) == 1 && BTN2_Bool == false) {
    Serial.println("BTN2 unpress");
    tone(Buzzer, 3500, 100);
    BTN2_Bool = true;
  }



  if (LED_bool) {
    myservo.write(0);
    ProtenVal = analogRead(POT);
    ProtenValMap = map(ProtenVal, 0, 1023, 0, 180);
    analogWrite(LED, ProtenValMap);
  } else {
    analogWrite(LED, 0);
    ProtenVal = analogRead(POT);
    ProtenValMap = map(ProtenVal, 0, 1023, 0, 255);
    myservo.write(ProtenValMap);
  }




  display.display();
}
float Get_Temp_Hum(char type) {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  if (type == 'T') {
    return temp.temperature;
  } else if (type == 'H') {
    return humidity.relative_humidity;
  } else {
    return 0;
  }
}