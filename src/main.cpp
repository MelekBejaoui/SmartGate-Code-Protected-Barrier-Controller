
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad_I2C.h>
#include <Wire.h>

#define I2CAddress 0x20
#define Relay 4


const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};


Keypad_I2C keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS,
  I2CAddress,
  PCF8574
);

// LCD 1604 (16 columns, 4 rows)
// RS, E, D4, D5, D6, D7
const int rs = 32;
const int en = 33;
const int d4 = 25;
const int d5 = 26;
const int d6 = 27;
const int d7 = 5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int count = 0;
char password[4] = {'1', '2', '5', '7'};
char input[4];

unsigned long startTime = 0;

void showEntryCode() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Entrer le code :");

  lcd.setCursor(0, 1);
  count = 0;
}

void showPasswordResult(bool res) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (res) {
    lcd.print("Code correct");
    digitalWrite(Relay, HIGH);
  } else {
    lcd.print("Code errone");
  }
}

void DeleteNumber() {
  count--;
  lcd.setCursor(count, 1);
  lcd.print(' ');
  lcd.setCursor(count, 1);
}

void ClearLcd() { 
  count = 0;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
}

void setup() {
  Wire.begin();

  lcd.begin(16, 4);
  lcd.clear();

  keypad.begin();

  showEntryCode();
  pinMode(Relay, OUTPUT);
}

void loop() {
  if (count == 4) {
    if (input[0] == password[0] &&
        input[1] == password[1] &&
        input[2] == password[2] &&
        input[3] == password[3]) {
      showPasswordResult(true);
    } else {
      showPasswordResult(false);
    }

    delay(3000);
    digitalWrite(Relay, LOW);
    showEntryCode();
    return;
  }

  char key = keypad.getKey();

  if (millis() - startTime > 7000 && count > 0) {
    ClearLcd();
  }

  if (key) {
    if (key == 'C') {
      if (count > 0) {
        DeleteNumber();
      }
      return;
    } else if (count < 4) {
      startTime = millis();
      lcd.print('*');
      input[count] = key;
      count++;
    }
  }
}
