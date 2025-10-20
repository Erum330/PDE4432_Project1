// --- Libraries ---
// Wire.h is needed for I2C communication
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// --- PIN DEFINITIONS ---
// The LCD uses the dedicated I2C pins:
// SDA -> Arduino Analog Pin A4
// SCL -> Arduino Analog Pin A5

// Initialize the LCD with the I2C address, 16 columns, 2 rows
// NOTE: 0x3F is a common address. If it doesn't work, try 0x27 or 0x20.
// **ACTION: Changed address to the final common address: 0x20**
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // Initialize the LCD
  lcd.init();
  
  // Turn on the backlight (Crucial for visibility)
  lcd.backlight();
  
  // Set the cursor to the first column (0) of the first row (0)
  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");
  
  // Move cursor to the second row (1)
  lcd.setCursor(0, 1);
  lcd.print("Basic Code Test");
}

void loop() {
  // Nothing needs to be constantly updated in the loop for a basic test.
  // The display remains static.
}
