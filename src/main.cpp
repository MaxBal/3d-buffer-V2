#include <Arduino.h>
// #define MENU_SELECTED_H 10 // высота элемента меню
// #define MENU_PARAMS_LEFT_OFFSET 92 // смещение вправо для рендеринга значений
// #define MENU_PAGE_ITEMS_COUNT 6 // Количесвто элементов меню на одной странице
// #define MENU_FAST_K 4 // коэффициент для ускоренного приращения (см. isFast ниже)


//----------------------------------------------------------------------------

#define BUFFER_INC      1
#define BUFFER_DEFAULT  100
#define BUFFER_MIN      0
#define BUFFER_MAX      1000


#define COUNT_DIFERENT_DRIVERS 12

#define DRIVER_INC 1
#define DRIVER_DEFAULT DriverType::a4988
#define DRIVER_MIN 0
#define DRIVER_MAX COUNT_DIFERENT_DRIVERS - 1



#define COUNT_DIFERENT_MICROSTEP 9

#define MICROSTEP_RESOLUTION_INC 1
#define MICROSTEP_RESOLUTION_DEFAULT MicrostepResolution::StepFull
#define MICROSTEP_RESOLUTION_MIN 0
#define MICROSTEP_RESOLUTION_MAX COUNT_DIFERENT_MICROSTEP - 1

//----------------------------------------------------------------------------

#define ENABLE_MOD_DEFAULT_VALUE false

#define STEP_PER_MM_SETUP_INC     1
#define STEP_PER_MM_SETUP_DEFAULT 200
#define STEP_PER_MM_SETUP_MIN     0
#define STEP_PER_MM_SETUP_MAX     10000

#define STEP_SPEED_INC      10
#define STEP_SPEED_DEFAULT  300
#define STEP_SPEED_MIN      0
#define STEP_SPEED_MAX      2000

#define STEP_PER_MM_SETUP_INC     1
#define STEP_PER_MM_SETUP_DEFAULT 200
#define STEP_PER_MM_SETUP_MIN     0
#define STEP_PER_MM_SETUP_MAX     10000

#define STEP_ACCELERATION_INC     10
#define STEP_ACCELERATION_DEFAULT 300
#define STEP_ACCELERATION_MIN     0
#define STEP_ACCELERATION_MAX     2000

//----------------------------------------------------------------------------

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
OledMenu<8, GyverOLED<SSH1106_128x64>> main_menu(&oled);
OledMenu<5, GyverOLED<SSH1106_128x64>> menu_stepper(&oled);
GStepper<STEPPER2WIRE> Motor1(steps, step, dir, en);


bool isExtruded = false;
bool isRetracted = false;
bool isSavePreset = false;
bool isChangePreset = false;

bool isStepperSettings = false; //menu stepper settings
bool isMainMenu = true;         //menu main
bool isSelectPreset = false;    //menu select save


// boolean isTurn();						// возвращает true при любом повороте, сама сбрасывается в false
// boolean isRight();						// возвращает true при повороте направо, сама сбрасывается в false
// boolean isLeft();						// возвращает true при повороте налево, сама сбрасывается в false
// boolean isRightH();						// возвращает true при удержании кнопки и повороте направо, сама сбрасывается в false
// boolean isLeftH();						// возвращает true при удержании кнопки и повороте налево, сама сбрасывается в false
// boolean isFastR();						// возвращает true при быстром повороте
// boolean isFastL();						// возвращает true при быстром повороте

// boolean isPress();						// возвращает true при нажатии кнопки, сама сбрасывается в false
// boolean isRelease();					// возвращает true при отпускании кнопки, сама сбрасывается в false
// boolean isReleaseHold();				// возвращает true при отпускании кнопки после удержания, сама сбрасывается в false
// boolean isClick();						// возвращает true при нажатии и отпускании кнопки, сама сбрасывается в false
// boolean isHolded();						// возвращает true при удержании кнопки, сама сбрасывается в false
// boolean isHold();						// возвращает true при удержании кнопки, НЕ СБРАСЫВАЕТСЯ
// boolean isSingle();						// возвращает true при одиночном клике (после таймаута), сама сбрасывается в false
// boolean isDouble();						// возвращает true при двойном клике, сама сбрасывается в false



enum class MicrostepResolution;
enum class DriverType;

struct StepperSettings
{
  bool    enable_mod;
  int16_t step_per_mm;
  int16_t speed;
  int16_t acceleration;
} ;

struct Settings
{
  int16_t buff_amount;
  MicrostepResolution res;
  DriverType driver_t;
  StepperSettings stepperSettings;
} currentSettings;

//my_menu.addItem(PSTR("Setup steps per mm"));

enum class MicrostepResolution
{
  StepFull,
  Step1_2,
  Step1_4,
  Step1_8,
  Step1_16,
  Step1_32,
  Step1_64,
  Step1_128,
  Step1_256
};

enum class DriverType
{
  a4988   = 0,
  DRV8825 = 1,
  TMC21005V = 2,
  TMC2100 = 3,
  TMC2130 = 4,
  TMC2208 = 5,
  S6609   = 6,
  TMC2209 = 7, 
  TMC2225 = 8,
  TMC2226 = 9,
  TMC5160 = 10,
  TMC5161 = 11
};

MicrostepResolution DriverMaxResolution(DriverType& driver)
{
  switch (driver)
  {
    case DriverType::a4988:     return MicrostepResolution::Step1_16;
    case DriverType::DRV8825:   return MicrostepResolution::Step1_32;
    case DriverType::TMC21005V: 
    case DriverType::TMC2100:   
    case DriverType::TMC2130:   
    case DriverType::TMC2208:   
    case DriverType::S6609:     
    case DriverType::TMC2209:   
    case DriverType::TMC2225:   
    case DriverType::TMC2226:   
    case DriverType::TMC5160:   
    case DriverType::TMC5161:   return MicrostepResolution::Step1_256;
  }
  return MicrostepResolution::StepFull; //default value
}

constexpr const int GetResolution(const MicrostepResolution& resolution)
{
  return (resolution == MicrostepResolution::StepFull)   ? 1 :
          (resolution == MicrostepResolution::Step1_2)   ? 2 :
          (resolution == MicrostepResolution::Step1_4)   ? 4 :
          (resolution == MicrostepResolution::Step1_8)   ? 8 :
          (resolution == MicrostepResolution::Step1_16)  ? 16 :
          (resolution == MicrostepResolution::Step1_32)  ? 32 :
          (resolution == MicrostepResolution::Step1_64)  ? 64 :
          (resolution == MicrostepResolution::Step1_128) ? 128 :
          (resolution == MicrostepResolution::Step1_256) ? 256 :
                                                          1;
}

constexpr const char* GetResolutionStr(const MicrostepResolution& resolution)
{
  return  (resolution == MicrostepResolution::StepFull) ? "Full Step" :
          (resolution == MicrostepResolution::Step1_2)  ? "Step 1/2" :
          (resolution == MicrostepResolution::Step1_4)  ? "Step 1/4" :
          (resolution == MicrostepResolution::Step1_8)  ? "Step 1/8" :
          (resolution == MicrostepResolution::Step1_16) ? "Step 1/16" :
          (resolution == MicrostepResolution::Step1_32) ? "Step 1/32" :
          (resolution == MicrostepResolution::Step1_64) ? "Step 1/64" :
          (resolution == MicrostepResolution::Step1_128)? "Step 1/128" :
          (resolution == MicrostepResolution::Step1_256)? "Step 1/256" :
                                                          "Unknown";
}

constexpr const char* GetDriverStr(const DriverType& driver)
{
  return  (driver == DriverType::a4988)     ? "a4988" :
          (driver == DriverType::DRV8825)   ? "DRV8825" :
          (driver == DriverType::S6609)     ? "S6609" :
          (driver == DriverType::TMC21005V) ? "TMC21005V" :
          (driver == DriverType::TMC2100)   ? "TMC2100" :
          (driver == DriverType::TMC2130)   ? "TMC2130" :
          (driver == DriverType::TMC2208)   ? "TMC2208" :
          (driver == DriverType::TMC2209)   ? "TMC2209" :
          (driver == DriverType::TMC2225)   ? "TMC2225" :
          (driver == DriverType::TMC2226)   ? "TMC2226" :
          (driver == DriverType::TMC5160)   ? "TMC5160" :
          (driver == DriverType::TMC5161)   ? "TMC5161" :
                                              "Unknown";
}

void setDefaultSettings(Settings& settings)
{
  settings.buff_amount = BUFFER_DEFAULT;
  settings.driver_t    = DRIVER_DEFAULT;
  settings.res         = MICROSTEP_RESOLUTION_DEFAULT;

  settings.stepperSettings.enable_mod  = ENABLE_MOD_DEFAULT_VALUE; 
  settings.stepperSettings.speed       = STEP_SPEED_DEFAULT;
  settings.stepperSettings.step_per_mm = STEP_PER_MM_SETUP_DEFAULT;
  settings.stepperSettings.acceleration= STEP_ACCELERATION_DEFAULT;
}

template<uint16_t _MS_SIZE, typename TGyverOLED >
void init_main_menu(OledMenu<_MS_SIZE, TGyverOLED>* main_menu, Settings* settings)
{
  main_menu->addItem(PSTR("Extrude"), &isExtruded); //0
  main_menu->addItem(PSTR("Retract"), &isRetracted);//1

  main_menu->addItem(
    PSTR("Buffer Amount"),  //2  количество выдавливаемого пластика
    GM_N_INT(BUFFER_INC), 
    &(settings->buff_amount), 
    GM_N_INT(BUFFER_MIN), 
    GM_N_INT(BUFFER_MAX));

  main_menu->addItem(
  PSTR("Microstep Resolution"), //3 
  GM_N_INT(MICROSTEP_RESOLUTION_INC), 
  &((int)settings->res),  //тупиковая, нужно полностью переписать!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  GM_N_INT(MICROSTEP_RESOLUTION_MIN), 
  GM_N_INT(MICROSTEP_RESOLUTION_MAX));

  main_menu->addItem(
  PSTR("Select Driver"), //4
  GM_N_INT(DRIVER_INC), 
  &(settings->driver_t), //тупиковая, нужно полностью переписать!!!!!!!!!!!!!!!!!!!!!!
  GM_N_INT(DRIVER_MIN), 
  GM_N_INT(DRIVER_MAX));

  main_menu->addItem(PSTR("Stepper Settings"),  &isStepperSettings); //5
  main_menu->addItem(PSTR("Save Preset"),       &isSavePreset);     //6
  main_menu->addItem(PSTR("Select Preset"),     &isSelectPreset);  //7

  main_menu->showMenu(true);
}

template<uint16_t _MS_SIZE, typename TGyverOLED >
void init_stepper_menu(OledMenu<_MS_SIZE, TGyverOLED>* stepper_menu, StepperSettings* stepper_settings)
{
  stepper_menu->addItem(PSTR("BACK <-- "), &isMainMenu);
  stepper_menu->addItem(PSTR("BACK"), &(stepper_settings->enable_mod));
  stepper_menu->addItem(PSTR("BACK"));
  stepper_menu->addItem(PSTR("BACK"));
  stepper_menu->addItem(PSTR("BACK"));
  
  //my_menu->addItem(PSTR("Microstep Resolution"), MICROSTEP_RESOLUTION_INC, settings.); 
}

void setup() {
  oled.init();
  oled.clear();
  oled.update();
  oled.setContrast(127);
  oled.setPower(true);
  enc1.setType(TYPE1);
  sensor.setDebounce(50);
  sensor.setTimeout(300);
  sensor.setClickTimeout(300);
  sensor.setType(HIGH_PULL);
  sensor.setDirection(NORM_OPEN);

  setDefaultSettings(currentSettings);
  init_main_menu(&main_menu, &currentSettings);

}



// boolean onItemPrintOverride(const int index, const void* val, const byte valType) {
//   if (index == 3) //micro step
//   {
//     const char* str = GetResolutionStr(*(MicrostepResolution*)(val));
//     char printable[strlen(str) + 1];
//     strcpy(printable, str);
//     oled.print(printable);
//   } 
//   else if (index == 4) //driver
//   {

//   }
  
//   // возвращаем всегда `false`, если мы не собираемся для других пунктов меню принтить значение
//   return false;
// }

void onMainMenuItemChange(const int index, const void* val, const byte valType) {
  if (valType == VAL_ACTION) 
  {
    switch (index)
    {
      case 0: 
      if(!isRetracted)
        //выдавить пластик
      return;
      case 1:
      if(!isExtruded)
        //ретракт пластик
      return;
      case 2:
      
      return;
      case 3:
      
      return;
      case 4:
      
      return;
      case 5:
      
      return;
      case 6:
      
      return;
      case 7:
      
      return;
    };
  }
}

void loop() {
  enc1.tick();
  sensor.tick();
  if(enc1.isPress() || enc1.isTurn())
  {
    if(enc1.isClick())
    {
      main_menu.toggleChangeSelected();
      Serial.print("text");
    }
    else
    {
      if(enc1.isLeft())
        main_menu.selectPrev();
      else if(enc1.isRight())
        main_menu.selectNext();
    }
    
    main_menu.refresh();  //в конце любого действия после нажатия на энкодер, апдейт графики
  } 
  else if (sensor.isPress())
  {
    //выдача пластика
  }
}
