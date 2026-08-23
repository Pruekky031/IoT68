#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60pDS3Loj"
#define BLYNK_TEMPLATE_NAME "2568"
#define BLYNK_AUTH_TOKEN "fphqFOmuW71ii9Btfm7IUiaO0ZN9GSoI"

#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include <Servo.h>
Servo myservo;
BlynkTimer timer;
Adafruit_AHTX0 aht;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);


#define GreenLED 2
#define YellowLED 3
#define RedLED 4


char ssid[] = "Pruek";
char pass[] = "pruekpruek";

unsigned long lastRead = 0;
float Temp = 0;
float Hum = 0;

int Mode;
int ServoDelay;
unsigned long servomillis;
bool Servomode = true;
BLYNK_WRITE(V0) {
  //param.asInt();
  //param.asFloat();
  //param.asStr();
}
BLYNK_WRITE(V2) {
  ServoDelay = param.asInt();
  //param.asFloat();
  //param.asStr();
}
BLYNK_WRITE(V3) {
  Mode = param.asInt();
  //param.asFloat();
  //param.asStr();
}
void sendData() {
  Blynk.virtualWrite(V0, Temp);
  Blynk.virtualWrite(V1, Hum);
  if (Mode == 1) {
    Blynk.virtualWrite(V4, "ON");
  } else {
    Blynk.virtualWrite(V4, "OFF");
  }
}

void setup() {
  Serial.begin(115200);
  Wire1.begin();
  myservo.attach(9);

  if (!aht.begin(&Wire1)) {
    Serial.println("Not Found AHT20 !");
    while (1) delay(10);
  }
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }

  Serial.println("AHT20 Ready!");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendData);

  pinMode(GreenLED, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(RedLED, OUTPUT);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  Blynk.run();
  timer.run();

  if (millis() - lastRead >= 2000) {
    lastRead = millis();

    Temp = Get_Temp_Hum('T');
    Hum = Get_Temp_Hum('H');
    Serial.print("Temp: ");
    Serial.println(Temp);
    Serial.print("Humu: ");
    Serial.println(Hum);
  }



  display.clearDisplay();

  display.setCursor(0, 10);
  display.print("Temp: ");
  display.println(Temp);

  display.print("Humuid: ");
  display.println(Hum);
  display.println();

  display.display();

  if (Temp < 27) {
    Serial.println("Temp cold");
    digitalWrite(GreenLED, HIGH);
    digitalWrite(YellowLED, LOW);
    digitalWrite(RedLED, LOW);
    display.println("Temp cold");
    if (Mode == 1) {
      if (millis() - servomillis > ServoDelay) {
        Serial.println("Servo");
        servomillis = millis();
        if (Servomode) {
          myservo.write(0);
          Servomode = !Servomode;
        } else {
          myservo.write(180);
          Servomode = !Servomode;
        }
      }
    }

  } else if (Temp >= 27 && Temp < 29) {
    Serial.println("Temp normal");
    digitalWrite(GreenLED, LOW);
    digitalWrite(YellowLED, HIGH);
    digitalWrite(RedLED, LOW);
    delay(100);
    digitalWrite(YellowLED, LOW);
    delay(100);
    display.println("Temp normal");
    if (Mode == 1) {
      if (millis() - servomillis > ServoDelay) {
        Serial.println("Servo");
        servomillis = millis();
        if (Servomode) {
          myservo.write(0);
          Servomode = !Servomode;
        } else {
          myservo.write(180);
          Servomode = !Servomode;
        }
      }
    }

  } else if (Temp >= 29) {
    Serial.println("Temp HOT");

    digitalWrite(GreenLED, LOW);
    digitalWrite(YellowLED, LOW);
    digitalWrite(RedLED, HIGH);
    myservo.write(0);
    delay(100);
    digitalWrite(RedLED, LOW);
    myservo.write(180);
    delay(100);
    display.println("Temp HOT");



  } else {
    Serial.println("Error");
    digitalWrite(GreenLED, LOW);
    digitalWrite(YellowLED, LOW);
    digitalWrite(RedLED, LOW);
    display.println("Temp Error");
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