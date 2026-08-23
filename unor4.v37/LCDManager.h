#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Constants
#define MAX_LCDS 4  // Maximum number of LCDs you want to support

// LCD Management Structure
struct LCDInstance {
  LiquidCrystal_I2C* lcd;
  int address;
  bool initialized;
};

// Global variables (extern means they're defined elsewhere)
extern LCDInstance lcd_instances[MAX_LCDS];
extern int lcd_count;

// Function declarations
void initLCDManagement();
LiquidCrystal_I2C* getLCDByAddress(int address);
void writeLiquidCrystal(int address, String message1, String message2);
void writeLiquidCrystalSmart(int address, String message1, String message2);
void setBacklight(int address, int backlight);
void clearLCD(int address);
void setLCDCursor(int address, int col, int row);
void printToLCD(int address, int col, int row, String message);

#endif // LCD_MANAGER_H