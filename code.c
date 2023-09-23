#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int buttonUpPin = 2; // пин для кнопки меню вверх
const int buttonDownPin = 3; // пин для кнопки меню вниз
const int buttonSelectPin = 4; // пин для кнопки выбора пункта

// Инициализируем дисплей
LiquidCrystal_I2C lcd(32, 16, 2);

int menuState = 0; // текущее состояние меню
int timerValue = 60; // значение времени обратного отсчета по умолчанию

void setup() {
  pinMode(buttonUpPin, INPUT_PULLUP); // настраиваем пин кнопки меню вверх на вход с подтяжкой
  pinMode(buttonDownPin, INPUT_PULLUP); // настраиваем пин кнопки меню вниз на вход с подтяжкой
  pinMode(buttonSelectPin, INPUT_PULLUP); // настраиваем пин кнопки выбора пункта на вход с подтяжкой
  Wire.begin(); // инициализируем I2C шину
  lcd.init(); // инициализируем дисплей
  lcd.backlight(); // включаем подсветку дисплея
}

void loop() {
  int buttonUpState = digitalRead(buttonUpPin); // считываем состояние кнопки меню вверх
  int buttonDownState = digitalRead(buttonDownPin); // считываем состояние кнопки меню вниз
  int buttonSelectState = digitalRead(buttonSelectPin); // считываем состояние кнопки выбора пункта

  // обрабатываем нажатие кнопки меню вверх
  if (buttonUpState == LOW) {
    if (menuState == 0) {
      menuState = 1; // переключаемся на пункт меню "Установить время"
    } else if (menuState == 1) {
      timerValue++; // увеличиваем значение времени обратного отсчета при выборе пункта "Добавить"
    }
    delay(200); // задержка для подавления дребезга контактов кнопки
  }

  // обрабатываем нажатие кнопки меню вниз
  if (buttonDownState == LOW) {
    if (menuState == 0) {
      menuState = 1; // переключаемся на пункт меню "Установить время"
    } else if (menuState == 1) {
      timerValue--; // уменьшаем значение времени обратного отсчета при выборе пункта "Убавить"
    }
    delay(200); // задержка для подавления дребезга контактов кнопки
  }

  // обрабатываем нажатие кнопки выбора пункта
  if (buttonSelectState == LOW) {
    if (menuState == 0) {
      // здесь можно добавить код для выполнения обратного отсчета
    } else if (menuState == 1) {
      menuState = 0; // возвращаемся в главное меню
    }
    delay(200); // задержка для подавления дребезга контактов кнопки
  }

  // выводим текущее состояние меню и значение времени обратного отсчета на дисплей
  lcd.setCursor(0, 0);
  lcd.print("Menu state: ");
  if (menuState == 0) {
    lcd.print("Main");
  } else if (menuState == 1) {
    lcd.print("Set time");
  }
  lcd.setCursor(0, 1);
  lcd.print("Timer value: ");
  lcd.print(timerValue);
}
