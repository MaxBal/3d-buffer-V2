#include <Arduino.h>
// #define MENU_SELECTED_H 10 // высота элемента меню
// #define MENU_PARAMS_LEFT_OFFSET 92 // смещение вправо для рендеринга значений
// #define MENU_PAGE_ITEMS_COUNT 6 // Количесвто элементов меню на одной странице
// #define MENU_FAST_K 4 // коэффициент для ускоренного приращения (см. isFast ниже)
#define SENSOR_PIN 5
#define CLK 2
#define DT 3
#define SW 4
#define SMOOTH_ALGORITHM
#include <GyverStepper.h>
#include "GyverEncoder.h"
#include "GyverButton.h"
#include <GyverOLED.h>
#include "GyverOLEDMenu.h"

#define steps 200
#define step 5
#define dir 6
#define en 7

GButton sensor(SENSOR_PIN);
Encoder enc1(CLK, DT, SW);
GyverOLED<SSH1106_128x64> oled;
OledMenu<8, GyverOLED<SSH1106_128x64>> menu(&oled);
OledMenu<2, GyverOLED<SSH1106_128x64>> menu_stepper(&oled);
GStepper<STEPPER2WIRE> Motor1(steps, step, dir, en);


void setup() {
  oled.init();
  void clear();
  oled.setContrast(127);
  oled.setPower(true);
  enc1.setType(TYPE1);
  sensor.setDebounce(50);
  sensor.setTimeout(300);
  sensor.setClickTimeout(300);
  sensor.setType(HIGH_PULL);
  sensor.setDirection(NORM_OPEN);
}

void loop() {
  menu.addItem("Extrude");

}
