#include "LCDManager.h"

// Global LCD management variables
LCDInstance lcd_instances[MAX_LCDS];
int lcd_count = 0;

// Initialize the LCD management system
void initLCDManagement() {
  for (int i = 0; i < MAX_LCDS; i++) {
    lcd_instances[i].lcd = nullptr;
    lcd_instances[i].address = -1;
    lcd_instances[i].initialized = false;
  }
  lcd_count = 0;
}

// Find or create LCD instance by address
LiquidCrystal_I2C* getLCDByAddress(int address) {
  // First, try to find existing LCD with this address
  for (int i = 0; i < lcd_count; i++) {
    if (lcd_instances[i].address == address && lcd_instances[i].initialized) {
      return lcd_instances[i].lcd;
    }
  }
  
  // If not found and we have space, create new one
  if (lcd_count < MAX_LCDS) {
    // Create new LCD instance
    lcd_instances[lcd_count].lcd = new LiquidCrystal_I2C(address, 16, 2);
    lcd_instances[lcd_count].address = address;
    lcd_instances[lcd_count].lcd->begin();  // Initialize only once
    lcd_instances[lcd_count].initialized = true;
    
    LiquidCrystal_I2C* newLCD = lcd_instances[lcd_count].lcd;
    lcd_count++;
    
    return newLCD;
  }
  
  // No space available - could add error handling here
  return nullptr;
}

// Helper function to pad string to specified length
String padString(String str, int length) {
  while (str.length() < length) {
    str += " ";  // Add spaces to fill the remaining characters
  }
  if (str.length() > length) {
    str = str.substring(0, length);  // Truncate if too long
  }
  return str;
}

// Write to LCD (replaces your existing function)
void writeLiquidCrystal(int address, String message1, String message2) {
  LiquidCrystal_I2C* lcd = getLCDByAddress(address);
  if (lcd != nullptr) {
    lcd->setCursor(0, 0);
    lcd->print(message1);
    lcd->setCursor(0, 1);
    lcd->print(message2);
  }
}

// Alternative: Smart update - only update what changed (MOST PROFESSIONAL)
void writeLiquidCrystalSmart(int address, String message1, String message2) {
  static String lastMessage1[MAX_LCDS] = {""};
  static String lastMessage2[MAX_LCDS] = {""};
  
  // Find LCD index for this address
  int lcdIndex = -1;
  for (int i = 0; i < lcd_count; i++) {
    if (lcd_instances[i].address == address) {
      lcdIndex = i;
      break;
    }
  }
  
  LiquidCrystal_I2C* lcd = getLCDByAddress(address);
  if (lcd != nullptr && lcdIndex != -1) {
    // Update line 1 only if changed
    if (lastMessage1[lcdIndex] != message1) {
      String paddedMessage1 = padString(message1, 16);
      lcd->setCursor(0, 0);
      lcd->print(paddedMessage1);
      lastMessage1[lcdIndex] = message1;
    }
    
    // Update line 2 only if changed
    if (lastMessage2[lcdIndex] != message2) {
      String paddedMessage2 = padString(message2, 16);
      lcd->setCursor(0, 1);
      lcd->print(paddedMessage2);
      lastMessage2[lcdIndex] = message2;
    }
  }
}

// Set LCD backlight
void setBacklight(int address, int backlight) {
  LiquidCrystal_I2C* lcd = getLCDByAddress(address);
  if (lcd != nullptr) {
    if (backlight == 1) {
      lcd->backlight();
    } else {
      lcd->noBacklight();
    }
  }
}

// Clear LCD screen
void clearLCD(int address) {
  LiquidCrystal_I2C* lcd = getLCDByAddress(address);
  if (lcd != nullptr) {
    lcd->clear();
  }
}

// Set cursor position
void setLCDCursor(int address, int col, int row) {
  LiquidCrystal_I2C* lcd = getLCDByAddress(address);
  if (lcd != nullptr) {
    lcd->setCursor(col, row);
  }
}

// Print to specific position
void printToLCD(int address, int col, int row, String message) {
  LiquidCrystal_I2C* lcd = getLCDByAddress(address);
  if (lcd != nullptr) {
    lcd->setCursor(col, row);
    lcd->print(message);
  }
}