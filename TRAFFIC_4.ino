#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define ledA1 11
#define ledA2 7
#define ledA3 3

#define ledB1 12
#define ledB2 8
#define ledB3 4

#define ledC1 9
#define ledC2 5
#define ledC3 1

#define ledD1 10
#define ledD2 6
#define ledD3 2

// Define IR sensor pins
#define sensorA A0
#define sensorB A1
#define sensorC A2
#define sensorD A3

int a, b, c, d;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x3F to your I2C address if different
SoftwareSerial gsmSerial(0, 1);      // RX, TX pins for GSM module

void setup() {
  Serial.begin(9600);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Print a test message on the LCD to verify it works
  lcd.setCursor(0, 0);
  lcd.print("Traffic Control");
  lcd.setCursor(0, 1);
  lcd.print("System Init");

  delay(2000);  // Display this message for 2 seconds

  // Initialize the LED pins as outputs
  pinMode(ledA1, OUTPUT);
  pinMode(ledA2, OUTPUT);
  pinMode(ledA3, OUTPUT);

  pinMode(ledB1, OUTPUT);
  pinMode(ledB2, OUTPUT);
  pinMode(ledB3, OUTPUT);

  pinMode(ledC1, OUTPUT);
  pinMode(ledC2, OUTPUT);
  pinMode(ledC3, OUTPUT);

  pinMode(ledD1, OUTPUT);
  pinMode(ledD2, OUTPUT);
  pinMode(ledD3, OUTPUT);

  // Initialize the sensor pins as inputs
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(sensorC, INPUT);
  pinMode(sensorD, INPUT);
}

void loop() {
  readSensor();

  // Check all sensors for high state to send an alert
  if (a == 1 && b == 1 && c == 1 && d == 1) {
    Serial.println("All IR sensors high. Traffic alert!");
  }

  // Traffic light control logic
  if (b == 1) {
    roadBopen();
  } else if (d == 1) {
    roadDopen();
    if (b == 1) {
      roadBopen();
    }
  } else if (a == 1) {
    roadAopen();
    if (b == 1) {
      roadBopen();
    } else if (d == 1) {
      roadDopen();
    }
  } else if (c == 1) {
    roadCopen();
    if (b == 1) {
      roadBopen();
    } else if (d == 1) {
      roadDopen();
    }
  } else {
    roadBopen(); // Default to opening road B if no other conditions are met
    if (a == 0 && b == 0 && c == 0 && d == 0) {
      roadDopen();
    }
    if (a == 0 && b == 0 && c == 0 && d == 0) {
      roadAopen();
    }
    if (a == 0 && b == 0 && c == 0 && d == 0) {
      roadCopen();
    }
  }
}

void readSensor() {
  a = digitalRead(sensorA);
  b = digitalRead(sensorB);
  c = digitalRead(sensorC);
  d = digitalRead(sensorD);

  Serial.print("Sensor States - A: ");
  Serial.print(a);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" C: ");
  Serial.print(c);
  Serial.print(" D: ");
  Serial.println(d);

  // Update LCD with sensor states
  lcd.clear(); // Clear the display before updating
  lcd.setCursor(0, 0);
  lcd.print("LANEA:");
  lcd.print(a);
  lcd.print(" LANEB:");
  lcd.print(b);
  lcd.setCursor(0, 1);
  lcd.print("LANEC:");
  lcd.print(c);
  lcd.print(" LANED:");
  lcd.print(d);
}

void roadAopen() {
  digitalWrite(ledA3, LOW);

  digitalWrite(ledA1, HIGH);
  digitalWrite(ledB3, HIGH);
  digitalWrite(ledC3, HIGH);
  digitalWrite(ledD3, HIGH);
  delay(5000);
  digitalWrite(ledA1, LOW);
  digitalWrite(ledA2, HIGH);
  delay(1000);
  digitalWrite(ledA2, LOW);
}

void roadBopen() {
  digitalWrite(ledB3, LOW);

  digitalWrite(ledA3, HIGH);
  digitalWrite(ledB1, HIGH);
  digitalWrite(ledC3, HIGH);
  digitalWrite(ledD3, HIGH);
  delay(5000);
  digitalWrite(ledB1, LOW);
  digitalWrite(ledB2, HIGH);
  delay(1000);
  digitalWrite(ledB2, LOW);
}

void roadCopen() {
  digitalWrite(ledC3, LOW);

  digitalWrite(ledA3, HIGH);
  digitalWrite(ledB3, HIGH);
  digitalWrite(ledC1, HIGH);
  digitalWrite(ledD3, HIGH);
  delay(5000);
  digitalWrite(ledC1, LOW);
  digitalWrite(ledC2, HIGH);
  delay(1000);
  digitalWrite(ledC2, LOW);
}

void roadDopen() {
  digitalWrite(ledD3, LOW);

  digitalWrite(ledA3, HIGH);
  digitalWrite(ledB3, HIGH);
  digitalWrite(ledC3, HIGH);
  digitalWrite(ledD1, HIGH);
  delay(5000);
  digitalWrite(ledD1, LOW);
  digitalWrite(ledD2, HIGH);
  delay(1000);
  digitalWrite(ledD2, LOW);
}
