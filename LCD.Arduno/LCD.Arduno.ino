#include <LiquidCrystal.h>

// LCD pins <--> Arduino pins
const int RS = 7, EN = 8, D4 = 9, D5 = 10, D6 = 11, D7 = 12;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// LED pins
const int ledPin1 = 5; // Pin for LED that will fade
const int ledPin2 = 6; // Pin for LED that will blink

// State variables to track text visibility
bool isAhmadVisible = false;
bool isMasaVisible = false;

// Timing variables for blinking
unsigned long previousMillisAhmad = 0;
unsigned long previousMillisMasa = 0;
unsigned long previousMillisBlink = 0;
const long interval = 1000; // Interval at which to blink (milliseconds)
const long blinkInterval = 100; // Interval at which LED2 blinks (milliseconds)

// Heart character data
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

// Variables for fading LED
int brightness = 0; // LED brightness level
int fadeAmount = 5; // Amount to change brightness

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.createChar(0, heart); // Create custom heart character
  lcd.clear(); // Clear any previous text

  // Initialize LED pins
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Toggle Ahmad visibility based on timing
  if (currentMillis - previousMillisAhmad >= interval) {
    previousMillisAhmad = currentMillis;
    isAhmadVisible = !isAhmadVisible;
  }

  // Toggle Masa visibility based on timing
  if (currentMillis - previousMillisMasa >= interval) {
    previousMillisMasa = currentMillis;
    isMasaVisible = !isMasaVisible;
  }

  // Update LCD display based on visibility states
  lcd.setCursor(0, 0); // Move cursor to (0, 0)
  if (isAhmadVisible) {
    lcd.print("Ahmad");
  } else {
    lcd.print("     ");
  }

  lcd.setCursor(5, 0); // Move cursor to (5, 0) for heart
  if (isAhmadVisible && isMasaVisible) {
    lcd.write(byte(0)); // Display heart character
  } else {
    lcd.print(" ");
  }

  lcd.setCursor(6, 0); // Move cursor to (6, 0)
  if (isMasaVisible) {
    lcd.print("Masa");
  } else {
    lcd.print("    ");
  }

  // Update LED1 (fade in and out)
  analogWrite(ledPin1, brightness);
  brightness += fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount; // Reverse the direction of the fade
  }
  delay(30); // Wait for 30 milliseconds to see the dimming effect

  // Update LED2 (blink)
  if (currentMillis - previousMillisBlink >= blinkInterval) {
    previousMillisBlink = currentMillis;
    int ledState = digitalRead(ledPin2);
    digitalWrite(ledPin2, !ledState);
  }
}
