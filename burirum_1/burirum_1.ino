#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <Servo.h>

Servo myservo;
Adafruit_AHTX0 aht;

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
#define POT A1
bool BTN1_Bool = true;
bool BTN2_Bool = true;
bool DoubleBTN = true;

bool OLED_COLOR_MODE = false;
unsigned long Color_Time = 0;

bool Select_Mode = false;
unsigned long Select_Mode_Time = 0;

bool Cancel_Select_Mode = false;
unsigned long Cancel_Select_Mode_Time = 0;




int Pot = 0;
int WindSpeed = 0;
int Mode = 0;


void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  matrix.begin();
  matrix.beginDraw();
  matrix.textScrollSpeed(60);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);

  myservo.attach(9);

  Wire.begin();

  if (!aht.begin(&Wire)) {
    Serial.println("Not Found AHT20 !");
    while (1)
      delay(10);
  }
  Serial.println("AHT20 Ready!");
}


void loop() {
  display.clearDisplay();

  Pot = analogRead(POT);
  WindSpeed = map(Pot, 0, 1023, 0, 10);

  if (millis() - lastRead >= 2000) {
    lastRead = millis();

    Temp = Get_Temp_Hum('T');
    Hum = Get_Temp_Hum('H');

    Serial.print("Temp: ");
    Serial.println(Temp);
    Serial.print("Humu: ");
    Serial.println(Hum);
  }



  if (!Select_Mode) {
    Cancel_Select_Mode = false;
    if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
      Serial.println("BTN1 press");

      Select_Mode_Time = millis();

      BTN1_Bool = false;
    } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
      Serial.print("Mode: ");
      Serial.println(Mode);

      if (millis() - Select_Mode_Time > 2000) {
        Select_Mode = true;
        Serial.println("Has Select Mode");
      } else {
        Mode += 1;
        if (Mode > 4) {
          Mode = 0;
        }
      }
      Serial.println("BTN1 unpress");
      BTN1_Bool = true;
    }


    if (digitalRead(BTN2) == 0 && BTN2_Bool == true) {
      Serial.println("BTN2 press");

      Color_Time = millis();

      BTN2_Bool = false;
    } else if (digitalRead(BTN2) == 1 && BTN2_Bool == false) {
      Serial.print("Mode: ");
      Serial.println(Mode);
      if (millis() - Color_Time > 2000) {
        OLED_COLOR_MODE = !OLED_COLOR_MODE;

        Serial.println("Has Change Mode Color ");
      } else {
        Mode -= 1;
        if (Mode < 0) {
          Mode = 4;
        }
      }
      Serial.println("BTN2 unpress");
      BTN2_Bool = true;
    }







    if (!BTN2_Bool) {
      if (millis() - Color_Time > 2000) tone(Buzz, 500);
    } else if (!BTN1_Bool) {
      if (millis() - Select_Mode_Time > 2000) tone(Buzz, 500);
    } else {
      noTone(Buzz);
    }
    if (Select_Mode) {
      if (Mode == 0) {

        matrix.println("   Monitor");
        matrix.endText(SCROLL_LEFT);
      } else if (Mode == 1) {

        matrix.println("   Temperature");
        matrix.endText(SCROLL_LEFT);
      } else if (Mode == 2) {

        matrix.println("   Humidity");
        matrix.endText(SCROLL_LEFT);
      } else if (Mode == 3) {

        matrix.println("   Hybrid");
        matrix.endText(SCROLL_LEFT);
      } else if (Mode == 4) {

        matrix.println("   Wind");
        matrix.endText(SCROLL_LEFT);
      }
    }



    display.setCursor(35, 0);
    display.print("Select Mode");


    if (Mode < 3) {
      display.fillTriangle(
        3 + 4, Mode * 15 + 2 + 15,
        3, Mode * 15 + 15,
        3, Mode * 15 + 4 + 15,
        WHITE);

      display.setCursor(12, 15);
      display.print("Monitor");

      display.setCursor(12, 30);
      display.print("Temperature");

      display.setCursor(12, 45);
      display.print("Humidity");
    } else {
      display.fillTriangle(
        3 + 4, (Mode - 3) * 15 + 2 + 15,
        3, (Mode - 3) * 15 + 15,
        3, (Mode - 3) * 15 + 4 + 15,
        WHITE);
      display.setCursor(12, 15);
      display.print("Hybrid");

      display.setCursor(12, 30);
      display.print("Wind");
    }

    if (OLED_COLOR_MODE) {
      display.invertDisplay(true);
    } else {
      display.invertDisplay(false);
    }

    display.display();
  }

  if (Select_Mode) {

    if (digitalRead(BTN1) == 0 && digitalRead(BTN2) == 0 && DoubleBTN == true) {
      Serial.println("Double BTN Press");
      Cancel_Select_Mode_Time = millis();

      DoubleBTN = false;
    } else if (digitalRead(BTN1) == 1 && digitalRead(BTN2) == 1 && DoubleBTN == false) {
      Serial.println("Double BTN UnPress");
      if (millis() - Cancel_Select_Mode_Time > 2000) {
        Select_Mode = false;
      }
      DoubleBTN = true;
    }
    if (!DoubleBTN) {
      if (millis() - Cancel_Select_Mode_Time > 2000) tone(Buzz, 500);
    } else {
      noTone(Buzz);
    }


    if (Mode == 0) {

      display.setCursor(35, 0);
      display.print("Monitor");
      display.setCursor(5, 15);
      display.print("T = ");
      display.print(Temp);
      display.print(" C");

      display.setCursor(5, 30);
      display.print("H = ");
      display.print(Hum);
      display.print("%");

      display.setCursor(5, 45);
      display.print("W = ");
      display.print(WindSpeed);
      display.print(" m/s");


    } else if (Mode == 1) {
      display.setCursor(25, 0);
      display.print("Temperature");

      display.setCursor(3, 15);
      display.print(Temp);
      display.print(" C");

      display.setCursor(3, 30);
      display.print("S = ");
      if (Temp >= 29) {
        display.print("Hot");

      } else if (Temp >= 27) {
        display.print("warm");
      } else {
        display.print("Normal");
      }

      display.setCursor(3, 45);
      display.print("degree = ");

      if (Temp >= 29) {
        display.print("90");
        myservo.write(90);

      } else if (Temp >= 27) {
        display.print("45");
        myservo.write(45);
      } else {
        display.print("0");
        myservo.write(0);
      }

    } else if (Mode == 2) {
      display.setCursor(35, 0);
      display.print("Humunity");

      display.setCursor(3, 15);
      display.print(Hum);
      display.print("%");

      display.setCursor(3, 30);
      display.print("degree = ");
      if (Hum > 80) {
        display.print("180");
        myservo.write(180);

      } else {
        display.print("0");
        myservo.write(0);
      }

    } else if (Mode == 3) {
      display.setCursor(35, 0);
      display.print("Hybrid");
      display.setCursor(5, 15);
      display.print("T = ");
      display.print(Temp);
      display.print(" C");

      display.setCursor(5, 30);
      display.print("H = ");
      display.print(Hum);
      display.print("%");

      display.setCursor(5, 45);
      display.print("degree = ");
      if (Temp >= 29 && Hum > 80) {

        display.print("135");
        myservo.write(135);

      } else if (Temp >= 27 || Hum > 70) {

        display.print("90");
        myservo.write(90);

      } else if (Temp <= 25 && Hum <= 70) {

        display.print("0");
        myservo.write(0);
      } else {
        display.print("0");
        myservo.write(0);
      }




    } else if (Mode == 4) {

      display.setCursor(45, 0);
      display.print("Wind");


      display.setCursor(5, 15);
      display.print("Speed = ");
      display.print(WindSpeed);

      display.print(" m/s");
      display.setCursor(5, 30);
      display.print("degree = ");

      if (WindSpeed >= 8) {
        myservo.write(160);
      } else if (WindSpeed >= 4) {
        myservo.write(80);
      } else if (WindSpeed >= 0) {
        myservo.write(0);
      }
      display.print(map(Pot, 0, 1023, 0, 180));
    }
    display.display();
  }
  display.display();
}




////////////////////////////////////////////////
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
