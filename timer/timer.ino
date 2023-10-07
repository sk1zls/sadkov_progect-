#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Encoder encoder(A3, A2);

const int joySelPin = A1;
const int speakerPin = 8;

unsigned long timerStart = 0;
unsigned long elapsedTime = 0;
unsigned long setTime = 0;
bool counting = false;
bool paused = false;
unsigned long pausedTime = 0;
unsigned long pausedSetTime = 0; // Добавляем переменную для сохранения установленного времени перед паузой

void setup() {
  pinMode(joySelPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Set time (mins): ");
}

void loop() {
  long newPosition = encoder.read() / 4;

  if (newPosition != setTime) {
    setTime = newPosition;
    elapsedTime = setTime * 60;

    lcd.setCursor(0, 1);
    lcd.print("                 ");
    lcd.setCursor(0, 1);
    lcd.print(setTime);

    counting = false; // Обнуляем таймер при изменении установленного времени
  }

  int joySelState = digitalRead(joySelPin);

  if (joySelState == LOW) {
    delay(500);

    if (!counting) {
      if (!paused) {
        timerStart = millis();
      } else {
        // Если была пауза, продолжаем с сохраненным установленным временем
        timerStart = millis() - pausedTime;
        paused = false;
        setTime = pausedSetTime; // Устанавливаем время из сохраненного значения
      }

      counting = true;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Counting Down: ");
    } else {
      counting = false; // Останавливаем таймер при повторном нажатии
      paused = true;
      pausedTime = millis() - timerStart; // Запоминаем прошедшее время при паузе
      pausedSetTime = setTime; // Сохраняем установленное время перед паузой
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Paused: ");
    }
  }

  if (counting) {
    unsigned long currentTime = millis();
    unsigned long elapsedTimeMillis = currentTime - timerStart;
    elapsedTime = setTime * 60 - (elapsedTimeMillis / 1000);

    if (elapsedTime <= 0) {
      counting = false;

      for (int i = 0; i < 10; i++) {
        tone(speakerPin, 440, 100);
        delay(100);
        noTone(speakerPin);
        delay(100);
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time's up!");
      lcd.setCursor(0, 1);
      lcd.print("Press  restart button");
      return;
    }

    lcd.setCursor(0, 1);
    lcd.print(elapsedTime / 60);
    lcd.print(":");
    if ((elapsedTime % 60) < 10) {
      lcd.print("0");
    }
    lcd.print(elapsedTime % 60);

    delay(200);
  }
}