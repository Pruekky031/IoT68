#include "BH1750Manager.h"

// Global BH1750 sensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

// Initialize the BH1750 management system
void initBH1750Management() {
  LightSensor.begin();
}

// Read light intensity from BH1750 sensor
String readBH1750() {
  String lux = String(LightSensor.GetLightIntensity());
  return lux;
}