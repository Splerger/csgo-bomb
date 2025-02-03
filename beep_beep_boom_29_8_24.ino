#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h>

const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;

const int i2c_addr = 0x27;

LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

const byte ROWS = 4; 
const byte COLS = 4; 
int i = 0;
String text = "";
bool boomd = false;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);

  pinMode(10, OUTPUT);
  pinMode(11, INPUT_PULLUP);

  lcd.begin(16,2);
  lcd.setCursor(0,0);
}

void boom() {
  lcd.setCursor(0, 1);
  int num = 40;

  if (digitalRead(11) == HIGH) {
    text = "";
    i = 0;
    lcd.clear();
    return;
  }


  for (int b = 0; b <= num; b++) {
    delay(500);
    tone(10, 1000, 150);
    Serial.print(F("beep: "));
    Serial.println(b);

    lcd.clear();

    if (num - b < 10) {
      lcd.print("0:0");
      lcd.print(num - b);
    } else {
      lcd.print("0:");
      lcd.print(num - b);
    }
  }

  boomd = true;
  Serial.println("boom");

  lcd.clear();
  lcd.print("boom");
  tone(10, 1000, 2000);
  delay(1000);
  lcd.clear();

  text = "";
  i = 0;
  boomd = false;
}

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void loop(){
  char customKey = customKeypad.getKey();
  //Serial.println(digitalRead(11));

  if (text == "7355608") {
    if (!boomd) {
      boom();

      text = "";
      i = 0;
      lcd.clear();
    }
  }
  
  if (customKey) {

    if (customKey == 'C') {
      text = "";
      i = 0;
      lcd.clear();
    }

    if (customKey == 'D') {
      reboot();
    }

    if (i == 16) {
      text = "";
      i = 0;
      lcd.clear();
    }
    lcd.setCursor(i,0);
    lcd.print(customKey);
    i = i+1;

    text = text + customKey;

    Serial.println(text);

    delay(150);
    i = i-1;
    lcd.setCursor(i, 0);
    lcd.write("*");
    i = i+1;

    if (text == "C") {
      text = "";
      i = 0;
      lcd.clear();
    }
    
    tone(10, 1000, 150);
  }
}