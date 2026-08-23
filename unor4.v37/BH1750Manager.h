#ifndef BH1750_MANAGER_H
#define BH1750_MANAGER_H

#include <Arduino.h>
#include <BH1750FVI.h>

// Global variables (extern means they're defined elsewhere)
extern BH1750FVI LightSensor;

// Function declarations
void initBH1750Management();
String readBH1750();

#endif // BH1750_MANAGER_H