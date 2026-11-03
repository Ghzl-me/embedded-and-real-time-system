#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// تعریف پین‌ها
const uint8_t NS_R = 8,  NS_Y = 9,  NS_G = 10;
const uint8_t EW_R = 11, EW_Y = 12, EW_G = 13;

const uint8_t BTN_NS_UP   = 2;
const uint8_t BTN_NS_DOWN = 3;
const uint8_t BTN_EW_UP   = 4;
const uint8_t BTN_EW_DOWN = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// زمان‌ها (میلی‌ثانیه)
uint32_t nsGreenTime = 5000;
uint32_t ewGreenTime = 5000;
const uint32_t YELLOW_TIME = 2000;
const uint32_t ALL_RED_TIME = 1000; 
const uint32_t STEP = 1000;

// حالت‌های چراغ
enum Phase { NS_GREEN, NS_YELLOW, ALL_RED_1, EW_GREEN, EW_YELLOW, ALL_RED_2 };
Phase phase = NS_GREEN;
uint32_t phaseStart = 0;

// تابع دریافت زمان هر حالت
uint32_t phaseDuration(Phase p) {
  switch (p) {
    case NS_GREEN:  return nsGreenTime;
    case NS_YELLOW: return YELLOW_TIME;
    case ALL_RED_1: return ALL_RED_TIME;
    case EW_GREEN:  return ewGreenTime;
    case EW_YELLOW: return YELLOW_TIME;
    case ALL_RED_2: return ALL_RED_TIME;
  }
  return 1000;
}

// تابع رفتن به حالت بعدی
Phase nextPhase(Phase p) {
  switch (p) {
    case NS_GREEN:  return NS_YELLOW;
    case NS_YELLOW: return ALL_RED_1;
    case ALL_RED_1: return EW_GREEN;
    case EW_GREEN:  return EW_YELLOW;
    case EW_YELLOW: return ALL_RED_2;
    case ALL_RED_2: return NS_GREEN;
  }
  return NS_GREEN;
}

// روشن/خاموش کردن LED ها
void applyLights(Phase p) {
  digitalWrite(NS_G, p == NS_GREEN  ? HIGH : LOW);
  digitalWrite(NS_Y, p == NS_YELLOW ? HIGH : LOW);
  digitalWrite(NS_R, (p == EW_GREEN || p == EW_YELLOW || p == ALL_RED_1 || p == ALL_RED_2) ? HIGH : LOW);

  digitalWrite(EW_G, p == EW_GREEN  ? HIGH : LOW);
  digitalWrite(EW_Y, p == EW_YELLOW ? HIGH : LOW);
  digitalWrite(EW_R, (p == NS_GREEN || p == NS_YELLOW || p == ALL_RED_1 || p == ALL_RED_2) ? HIGH : LOW);
}

// مدیریت LCD
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("NS:");
  lcd.print(nsGreenTime / 1000);
  lcd.print("s  EW:");
  lcd.print(ewGreenTime / 1000);
  lcd.print("s    ");

  lcd.setCursor(0, 1);
  uint32_t elapsed = millis() - phaseStart;
  uint32_t remain = phaseDuration(phase) - elapsed;
  if (remain > phaseDuration(phase)) remain = 0; // جلوگیری از منفی شدن
  
  lcd.print("Time Left: ");
  lcd.print(remain / 1000);
  lcd.print("s    ");
}

// مدیریت دکمه‌ها (با دی‌بانس)
void checkButtons() {
  static uint32_t lastBtnTime = 0;
  if (millis() - lastBtnTime > 200) { // 200 میلی‌ثانیه تاخیر برای جلوگیری از چند بار فشردن
    if (digitalRead(BTN_NS_UP) == LOW)   { nsGreenTime += STEP; lastBtnTime = millis(); }
    if (digitalRead(BTN_NS_DOWN) == LOW && nsGreenTime > 2000) { nsGreenTime -= STEP; lastBtnTime = millis(); }
    if (digitalRead(BTN_EW_UP) == LOW)   { ewGreenTime += STEP; lastBtnTime = millis(); }
    if (digitalRead(BTN_EW_DOWN) == LOW && ewGreenTime > 2000) { ewGreenTime -= STEP; lastBtnTime = millis(); }
  }
}

void setup() {
  pinMode(NS_R, OUTPUT); pinMode(NS_Y, OUTPUT); pinMode(NS_G, OUTPUT);
  pinMode(EW_R, OUTPUT); pinMode(EW_Y, OUTPUT); pinMode(EW_G, OUTPUT);

  pinMode(BTN_NS_UP,   INPUT_PULLUP);
  pinMode(BTN_NS_DOWN, INPUT_PULLUP);
  pinMode(BTN_EW_UP,   INPUT_PULLUP);
  pinMode(BTN_EW_DOWN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  phase = NS_GREEN;
  phaseStart = millis();
  applyLights(phase);
}

void loop() {
  checkButtons();

  if (millis() - phaseStart >= phaseDuration(phase)) {
    phase = nextPhase(phase);
    phaseStart = millis();
    applyLights(phase);
  }

  static uint32_t lastLcdUpdate = 0;
  if (millis() - lastLcdUpdate > 200) {
    lastLcdUpdate = millis();
    updateLCD();
  }
}