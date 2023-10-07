#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Encoder encoder(A3, A2);

const int joySelPin = A1;
const int speakerPin = 8;
const int buttonPin = 9; // Пин для кнопки включения/выключения звука

bool soundEnabled = true; // Переменная для отслеживания состояния звука (включен или выключен)

unsigned long timerStart = 0;
unsigned long elapsedTime = 0;
unsigned long setTime = 0;
bool counting = false;
bool paused = false;
unsigned long pausedTime = 0;
unsigned long pausedSetTime = 0;

void setup() {
  pinMode(joySelPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Устанавливаем кнопку с подтяжкой к питанию

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Set time (mins): ");
  lcd.setCursor(15, 0); // Позиция для вывода состояния звука
  lcd.print(soundEnabled ? "S" : "M"); // Вывод "S" или "M" в зависимости от состояния звука
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
      lcd.setCursor(15, 0); // Позиция для вывода состояния звука
      lcd.print(soundEnabled ? "S" : "M"); // Вывод "S" или "M" в зависимости от состояния звука
    } else {
      counting = false; // Останавливаем таймер при повторном нажатии
      paused = true;
      pausedTime = millis() - timerStart; // Запоминаем прошедшее время при паузе
      pausedSetTime = setTime; // Сохраняем установленное время перед паузой
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Paused: ");
      lcd.setCursor(15, 0); // Позиция для вывода состояния звука
      lcd.print(soundEnabled ? "S" : "M"); // Вывод "S" или "M" в зависимости от состояния звука
    }
  }

  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    // При нажатии кнопки меняем состояние звука и выводим соответствующее сообщение
    soundEnabled = !soundEnabled;
    lcd.setCursor(15, 0); // Позиция для вывода состояния звука
    lcd.print(soundEnabled ? "S" : "M"); // Вывод "S" или "M" в зависимости от состояния звука
    delay(500);  // Задержка для предотвращения дребезга кнопки
  }

  if (counting) {
    unsigned long currentTime = millis();
    unsigned long elapsedTimeMillis = currentTime - timerStart;
    elapsedTime = setTime * 60 - (elapsedTimeMillis / 1000);

    if (elapsedTime <= 0) {
      counting = false;

      for (int i = 0; i < 10; i++) {
        if (soundEnabled) {
          tone(speakerPin, 440, 100);
          delay(100);
          noTone(speakerPin);
          delay(100);
        }
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time's up!");
      lcd.setCursor(0, 1);
      lcd.print("Press restart button");
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
