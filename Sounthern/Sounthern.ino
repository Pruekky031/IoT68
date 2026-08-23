////////////////////////////////////////////// libary
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <Servo.h>
Servo myservo;
Adafruit_AHTX0 aht;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);
////////////////////////////////////////////// libary
////////////////////////////////////////////// define pin
#define BTN1 3
#define BTN2 4
#define SOUND_PIN 2
#define POT A1


////////////////////////////////////////////// define pin
////////////////////////////////////////////// varliable
int Mode = 0;
unsigned long lastRead = 0;
float TEM = 0;
float falenhigh;

int HumMap = 0;
int HUM = 0;
bool BTN_Bool = true;
bool BTN1_Bool = true;
bool BTN2_Bool = true;
int potenval;
int protenMap;
int lockMode = false;
////////////////////////////////////////////// varliable


void setup() {
  Serial.begin(115200);
  myservo.attach(9);

  Serial.println("AHT20 Ready!");
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  Serial.println("Start//");

  Wire1.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }
  if (!aht.begin(&Wire1)) {
    Serial.println("Not Found AHT20 !");
    while (1) delay(10);
  }
}

void loop() {


  if (millis() - lastRead >= 2000) {
    lastRead = millis();

    TEM = Get_Temp_Hum('T');
    HUM = Get_Temp_Hum('H');
  }


  if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
    Serial.println("BTN1 press");
    tone(SOUND_PIN, 2000, 100);

    BTN1_Bool = false;
  } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
    Serial.println("BTN1 unpress");
    delay(100);
    if (digitalRead(BTN2) == 1) {
      if (Mode == 0) {
        Mode = 1;
      } else if (Mode == 1) {
        Mode = 2;
      } else if (Mode == 2) {
        Mode = 3;
      } else if (Mode == 3) {
        Mode = 1;
      }
      Serial.println(Mode);
    }



    BTN1_Bool = true;
  }

  if (digitalRead(BTN2) == 0 && BTN2_Bool == true) {
    Serial.println("BTN2 press");
    tone(SOUND_PIN, 2000, 100);

    BTN2_Bool = false;
  } else if (digitalRead(BTN2) == 1 && BTN2_Bool == false) {
    Serial.println("BTN2 unpress");

    delay(100);
    if (digitalRead(BTN1) == 1) {
      if (Mode == 0) {
        Mode = 3;
      } else if (Mode == 1) {
        Mode = 3;
      } else if (Mode == 2) {
        Mode = 1;
      } else if (Mode == 3) {
        Mode = 2;
      }
      Serial.println(Mode);
    }


    BTN2_Bool = true;
  }

  if (digitalRead(BTN2) == 0 && digitalRead(BTN1) == 0 && BTN_Bool) {
    BTN_Bool = false;
    if (lockMode == true) {
      lockMode = false;
    } else {
      lockMode = true;
    }
  } else if (digitalRead(BTN2) == 1 && digitalRead(BTN1) == 1 && !(BTN_Bool)) {
    BTN_Bool = true;
  }
  if (Mode == 0) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("press");
    display.setCursor(0, 15);
    display.println("sw1 to humid");
    display.setCursor(0, 30);
    display.println("sw2 to temp");
    display.display();
  } else if (Mode == 1) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("mode : Humid");
    display.setCursor(0, 15);
    display.print(HUM);
    display.print(" %");
    HumMap = map(HUM, 0, 100, 0, 128);
    display.drawRect(0, 30, 128, 10, SSD1306_WHITE);
    display.fillRect(0, 30, HumMap, 10, SSD1306_WHITE);
    if (HUM > 70) {
      display.setCursor(0, 45);
      display.print("high humid!");
    }

    display.display();
  } else if (Mode == 2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("mode : Servo ");
    potenval = analogRead(POT);
    protenMap = map(potenval, 0, 1023, 180, 0);
    display.setCursor(0, 15);
    display.print(protenMap);
    display.print("  degree");

    if (lockMode) {

      display.setCursor(0, 45);
      display.print("LockMode");
    } else {

      myservo.write(protenMap);
    }
    display.display();
  } else if (Mode == 3) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("mode : Temp ");

    display.setCursor(0, 15);
    display.print(TEM);
    display.print(" C");

    falenhigh = (TEM * 9 / 5) + 32;

    display.setCursor(0, 30);
    display.print(falenhigh);
    display.print(" F");

    display.display();
    delay(250);
    if (TEM > 25) {
      display.setCursor(0, 45);
      display.println("HIGH Temp!");
      display.display();
      delay(250);
    }
  }
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
