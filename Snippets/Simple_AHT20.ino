#include <Adafruit_AHTX0.h>
#include <Wire.h>

Adafruit_AHTX0 aht;

unsigned long lastRead = 0;
float Temp = 0;
float Hum = 0;

void setup()
{
  Serial.begin(115200);
  Wire1.begin();

  if (!aht.begin(&Wire1))
  {
    Serial.println("Not Found AHT20 !");
    while (1)
      delay(10);
  }
  Serial.println("AHT20 Ready!");
}

void loop()
{
  if (millis() - lastRead >= 2000)
  {
    lastRead = millis();

    Temp = Get_Temp_Hum('T');
    Hum = Get_Temp_Hum('H');

    Serial.print("Temp: ");
    Serial.println(Temp);
    Serial.print("Humu: ");
    Serial.println(Hum);
  }
}
////////////////////////////////////////////////
float Get_Temp_Hum(char type)
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  if (type == 'T')
  {
    return temp.temperature;
  }
  else if (type == 'H')
  {
    return humidity.relative_humidity;
  }
  else
  {
    return 0;
  }
}