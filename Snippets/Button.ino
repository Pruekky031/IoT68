#define BTN1 2
#define BTN2 3
bool BTN1_Bool = true;
bool BTN2_Bool = true;

void setup() {
  Serial.begin(115200);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
}

void loop() {
  if (digitalRead(BTN1) == 0 && BTN1_Bool == true) {
    Serial.println("BTN1 press");
    BTN1_Bool = false;
  } else if (digitalRead(BTN1) == 1 && BTN1_Bool == false) {
    Serial.println("BTN1 unpress");
    BTN1_Bool = true;
  }

  if (digitalRead(BTN2) == 0 && BTN2_Bool == true) {
    Serial.println("BTN2 press");
    BTN2_Bool = false;
  } else if (digitalRead(BTN2) == 1 && BTN2_Bool == false) {
    Serial.println("BTN2 unpress");
    BTN2_Bool = true;
  }
}


