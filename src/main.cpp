
//#define MENU_SELECTED_H 20 // высота элемента меню
// #define MENU_PARAMS_LEFT_OFFSET 92 // смещение вправо для рендеринга значений
//#define MENU_PAGE_ITEMS_COUNT 6 // Количесвто элементов меню на одной странице
// #define MENU_FAST_K 4 // коэффициент для ускоренного приращения (см. isFast ниже)


//-------------------------- <Main Menu> ---------------------------------------

#define BUFFER_INC      1
#define BUFFER_DEFAULT  100
#define BUFFER_MIN      0
#define BUFFER_MAX      1000


#define COUNT_DIFERENT_DRIVERS static_cast<int>(DriverType::COUNT)

#define DRIVER_INC 1
#define DRIVER_DEFAULT static_cast<int>(DriverType::a4988)
#define DRIVER_MIN 0
#define DRIVER_MAX COUNT_DIFERENT_DRIVERS - 1  


#define COUNT_DIFERENT_MICROSTEP static_cast<int>(MicrostepResolution::COUNT)

#define MICROSTEP_RESOLUTION_INC 1
#define MICROSTEP_RESOLUTION_DEFAULT static_cast<int>(MicrostepResolution::StepFull)
#define MICROSTEP_RESOLUTION_MIN 0
#define MICROSTEP_RESOLUTION_MAX COUNT_DIFERENT_MICROSTEP - 1


#define EEPROM_OFFEST_BYTE_FOR_NUMERATION_SAVES 10

#define EEPROM_SAVE_PRESET_INC 1
#define EEPROM_SAVE_PRESET_DEFAULT 0
#define EEPROM_SAVE_PRESET_MIN 0
#define EEPROM_SAVE_PRESET_MAX (((1024 - EEPROM_OFFEST_BYTE_FOR_NUMERATION_SAVES) / sizeof(Settings)) - 1)

//-------------------------- <Stepper Menu> --------------------------------------

#define STEP_ENABLE_MOD_DEFAULT_VALUE false

#define STEP_PER_MM_SETUP_INC     1
#define STEP_PER_MM_SETUP_DEFAULT 200
#define STEP_PER_MM_SETUP_MIN     0
#define STEP_PER_MM_SETUP_MAX     10000

#define STEP_SPEED_INC      10
#define STEP_SPEED_DEFAULT  300
#define STEP_SPEED_MIN      0
#define STEP_SPEED_MAX      2000

#define STEP_ACCELERATION_INC     10
#define STEP_ACCELERATION_DEFAULT 300
#define STEP_ACCELERATION_MIN     0
#define STEP_ACCELERATION_MAX     2000

//------------------------------ <Основные настройки> ----------------------------------

#define TIME_BETWEEN_SENSORE_USES  500 //по умолчанию 500мс
#define SENSOR_PIN 5
#define CLK 2
#define DT 3
#define SW 4
#define SMOOTH_ALGORITHM

//#define MENU_PARAMS_LEFT_OFFSET 64
#define CUSTOM_MENU_LEFT_OFFSET 64
#define DEAD_EEPROM_CELL_OFFSET 0

#include <Arduino.h>
#include <GyverStepper.h>
#include "GyverEncoder.h"
#include "GyverButton.h"
#include <GyverOLED.h>
#include "GyverOLEDMenu.h"
#include <EEPROM.h>


// #define ENC_DEBOUNCE 80         // установка времени антидребезга (по умолчанию 80 мс)
// #define ENC_HOLD_TIMEOUT 300    // установка таймаута удержания (по умолчанию 300 мс)
// #define ENC_CLICK_TIMEOUT 500   // установка таймаута между кликами (по умолчанию 500 мс)	
// #define ENC_STEP_TIMEOUT 400    // установка таймаута между инкрементами (по умолчанию 400 мс)	
// #define ENC_TYPE HIGH_PULL      // установка типа кнопки (HIGH_PULL - подтянута к питанию, LOW_PULL - к gnd)	
// #define ENC_DIRECTION NORM_OPEN // установка направления (разомкнута/замкнута по умолчанию - NORM_OPEN, NORM_CLOSE)	

//void setDebounce(uint16_t debounce);				  // установка времени антидребезга (по умолчанию 80 мс)
//void setTimeout(uint16_t new_timeout);				// установка таймаута удержания (по умолчанию 300 мс)
//void setClickTimeout(uint16_t new_timeout);		// установка таймаута между кликами (по умолчанию 500 мс)	
//void setStepTimeout(uint16_t step_timeout);		// установка таймаута между инкрементами (по умолчанию 400 мс)	
//void setType(bool type);							        // установка типа кнопки (HIGH_PULL - подтянута к питанию, LOW_PULL - к gnd)	
//void setDirection(bool dir);						      // установка направления (разомкнута/замкнута по умолчанию - NORM_OPEN, NORM_CLOSE)	

#define steps 200
#define step 5
#define dirrr 6
#define en 7


#define EEPROM_REGULAR_START 1  // предполагать что EEPROM проинициилизированная
#define EEPROM_FIRST_START   2  // предполагать что EEPROM ещё не проинициилизированная для работы кода
#define EEPROM_AUTO_START    3  // перед каждым запуском условно проверять состояние инициилизации EEPROM (не сработает если ранее другие программы/прошивки использовали EEPROM в своих целях)

#define EEPROM_MODE EEPROM_FIRST_START //принцип запуска EEPROM


// boolean isTurn();						// возвращает true при любом повороте, сама сбрасывается в false
// boolean isRight();						// возвращает true при повороте направо, сама сбрасывается в false
// boolean isLeft();						// возвращает true при повороте налево, сама сбрасывается в false
// boolean isRightH();					// возвращает true при удержании кнопки и повороте направо, сама сбрасывается в false
// boolean isLeftH();						// возвращает true при удержании кнопки и повороте налево, сама сбрасывается в false
// boolean isFastR();						// возвращает true при быстром повороте
// boolean isFastL();						// возвращает true при быстром повороте

// boolean isPress();						// возвращает true при нажатии кнопки, сама сбрасывается в false
// boolean isRelease();					// возвращает true при отпускании кнопки, сама сбрасывается в false
// boolean isReleaseHold();			// возвращает true при отпускании кнопки после удержания, сама сбрасывается в false
// boolean isClick();						// возвращает true при нажатии и отпускании кнопки, сама сбрасывается в false
// boolean isHolded();					// возвращает true при удержании кнопки, сама сбрасывается в false
// boolean isHold();						// возвращает true при удержании кнопки, НЕ СБРАСЫВАЕТСЯ
// boolean isSingle();					// возвращает true при одиночном клике (после таймаута), сама сбрасывается в false
// boolean isDouble();					// возвращает true при двойном клике, сама сбрасывается в false



enum class MicrostepResolution;
enum class DriverType;
enum class MenuType;

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
  int16_t microstep_resolution;
  int16_t driver_type;
  StepperSettings stepperSettings;

  MicrostepResolution getMicrostepResolution() 
  {return static_cast<MicrostepResolution>(this->microstep_resolution);}

  DriverType getDriverType() 
  {return static_cast<DriverType>(this->driver_type);}

  void setMicrostepResolution(const MicrostepResolution& microstep_resolution) 
  {this->microstep_resolution = static_cast<int>(microstep_resolution);}

  void setDriverType(const DriverType& driver_type) 
  {this->driver_type = static_cast<int>(driver_type);}

} ;

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
  Step1_256,
  COUNT //всегда последний элемент
};

enum class DriverType
{
  a4988,
  DRV8825,
  TMC21005V,
  TMC2100,
  TMC2130,
  TMC2208,
  S6609,
  TMC2209, 
  TMC2225,
  TMC2226,
  TMC5160,
  TMC5161,
  COUNT //всегда последний элемент
};

enum class MenuType
{
  None,
  MainMenu,
  StepperMenu,
  SaveSelectionCustomMenu,
  COUNT //всегда последний элемент
};


GButton sensor(SENSOR_PIN);
Encoder enc1(CLK, DT, SW);
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled(0x3C);
OledMenu<9, GyverOLED<SSD1306_128x64, OLED_BUFFER>> main_menu(&oled);
OledMenu<5, GyverOLED<SSD1306_128x64, OLED_BUFFER>> stepper_menu(&oled);
GStepper<STEPPER2WIRE> Motor1(steps, step, dirrr, en);


// #include <new.h>
// byte custom_mem[sizeof(OledMenu<9, GyverOLED<SSD1306_128x64, OLED_BUFFER>>)];

// OledMenu<9, GyverOLED<SSD1306_128x64, OLED_BUFFER>>* main_menu_p =
// new (custom_mem) OledMenu<9, GyverOLED<SSD1306_128x64, OLED_BUFFER>>(&oled);

// OledMenu<5, GyverOLED<SSD1306_128x64, OLED_BUFFER>>* stepper_menu_p =
// new (custom_mem) OledMenu<5, GyverOLED<SSD1306_128x64, OLED_BUFFER>>(&oled);

Settings currentSettings;
StepperSettings& currentStepperSettings(currentSettings.stepperSettings);

Settings previousSettings;
StepperSettings& previousStepperSettings(previousSettings.stepperSettings);

int currentExtrude = 0;
unsigned long prevSensMillis = 0;

bool isTurnOnSensor  = false;
bool settingsChanged = false;
int lastEEEPROM_elem = 0;
int currEEEPROM_elem = 0;

MenuType currentMenuType = MenuType::None;


MicrostepResolution DriverMaxResolution(const DriverType& driver)
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
  return  (resolution == MicrostepResolution::StepFull)  ? 1 :
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

const char* GetResolutionStr(const MicrostepResolution& resolution)
{
  return  (resolution == MicrostepResolution::StepFull) ? PSTR("Full Step") :
          (resolution == MicrostepResolution::Step1_2)  ? PSTR("Step 1/2") :
          (resolution == MicrostepResolution::Step1_4)  ? PSTR("Step 1/4") :
          (resolution == MicrostepResolution::Step1_8)  ? PSTR("Step 1/8") :
          (resolution == MicrostepResolution::Step1_16) ? PSTR("Step 1/16") :
          (resolution == MicrostepResolution::Step1_32) ? PSTR("Step 1/32") :
          (resolution == MicrostepResolution::Step1_64) ? PSTR("Step 1/64") :
          (resolution == MicrostepResolution::Step1_128)? PSTR("Step 1/128") :
          (resolution == MicrostepResolution::Step1_256)? PSTR("Step 1/256") :
                                                          PSTR("Unknown");
}

const char* GetDriverStr(const DriverType& driver)
{
  return  (driver == DriverType::a4988)     ? PSTR("a4988") :
          (driver == DriverType::DRV8825)   ? PSTR("DRV8825") :
          (driver == DriverType::S6609)     ? PSTR("S6609") :
          (driver == DriverType::TMC21005V) ? PSTR("TMC21005V") :
          (driver == DriverType::TMC2100)   ? PSTR("TMC2100") :
          (driver == DriverType::TMC2130)   ? PSTR("TMC2130") :
          (driver == DriverType::TMC2208)   ? PSTR("TMC2208") :
          (driver == DriverType::TMC2209)   ? PSTR("TMC2209") :
          (driver == DriverType::TMC2225)   ? PSTR("TMC2225") :
          (driver == DriverType::TMC2226)   ? PSTR("TMC2226") :
          (driver == DriverType::TMC5160)   ? PSTR("TMC5160") :
          (driver == DriverType::TMC5161)   ? PSTR("TMC5161") :
                                              PSTR("Unknown");
}

inline void setDefaultSettings() 
{
  currentSettings.buff_amount         = BUFFER_DEFAULT;
  currentSettings.driver_type         = DRIVER_DEFAULT;
  currentSettings.microstep_resolution= MICROSTEP_RESOLUTION_DEFAULT;
  
  currentStepperSettings.enable_mod   = STEP_ENABLE_MOD_DEFAULT_VALUE;
  currentStepperSettings.step_per_mm  = STEP_PER_MM_SETUP_DEFAULT;
  currentStepperSettings.speed        = STEP_SPEED_DEFAULT;
  currentStepperSettings.acceleration = STEP_ACCELERATION_DEFAULT;
}


inline void Extrude(bool isReverse = false)
{
  Motor1.reverse(isReverse);
  
  //типо сам настраиваешь, но умножение на дробный шаг автоматическое (можно поменять)
  Motor1.setTarget(
    currentStepperSettings.step_per_mm * 
    currentSettings.buff_amount *
    GetResolution( max(currentSettings.getMicrostepResolution(), 
                      DriverMaxResolution(currentSettings.getDriverType())) ) //получаем enum с помощью статик каста и потом получаем правильное значение
   , RELATIVE);
}

//----------------main_menu callbacks---------------------------
void cbChangeSensorState(const int index, const void* val, const byte valType)
{
}

void cbExtrudeOrRetract(const int index, const void* val, const byte valType)
{
  if(currentExtrude < 0)
    Motor1.reverse(true);
  else
    Motor1.reverse(false);
  
  currentExtrude = abs(currentExtrude);
  //поменять на меню с крутилкой
  Motor1.setTarget(
    currentStepperSettings.step_per_mm * 
    currentExtrude *
    GetResolution(max(
      currentSettings.getMicrostepResolution(), 
      DriverMaxResolution(currentSettings.getDriverType()) ) ) //получаем enum с помощью статик каста и потом получаем правильное значение
   , RELATIVE);
}

void cbBufferAmount(const int index, const void* val, const byte valType)
{
  settingsChanged = true;
}

void cbMicrostepResolution(const int index, const void* val, const byte valType)
{
  settingsChanged = true;
}

void cbSelectDriver(const int index, const void* val, const byte valType)
{
  settingsChanged = true;
}

void cbStepperSettings(const int index, const void* val, const byte valType)
{
  currentMenuType = MenuType::StepperMenu;
  main_menu.showMenu(false);
  stepper_menu.showMenu(true);
}

//-----------------EEPROM_овские недоноски-----------------------------------
void cbSavePreset(const int index, const void* val, const byte valType)
{
  EEPROM.put<Settings>(lastEEEPROM_elem, currentSettings);
  ++lastEEEPROM_elem;
}

void cbSelectPreset(const int index, const void* val, const byte valType)
{
  //работа с EEPROM + процесс мышления динамической кнопки
}

//----------------stepper settings callbacks---------------------------

void cbBACK(const int index, const void* val, const byte valType)
{
  currentMenuType = MenuType::MainMenu;
  stepper_menu.showMenu(false);
  main_menu.showMenu(true);
}

void cbEnableMod(const int index, const void* val, const byte valType)
{
  
}

void cbStepPerMM(const int index, const void* val, const byte valType)
{
  
}

void cbSpeed(const int index, const void* val, const byte valType)
{
  
}



//----------------main menu custom print--------------------------------

//oled->setCursorXY(MENU_PARAMS_LEFT_OFFSET, _text_y);
//(const int index, const void* val, const byte valType);

inline void customLeftPadding(const int& item_index)
{
  #ifdef CUSTOM_MENU_LEFT_OFFSET
  oled.setCursorXY(CUSTOM_MENU_LEFT_OFFSET, item_index * MENU_SELECTED_H + MENU_ITEM_PADDING_TOP);
  #else
  oled.setCursorXY(MENU_PARAMS_LEFT_OFFSET, item_index);
  #endif
}

bool prExtrudeOrRetract(const int index, const void* val, const byte valType)
{
  oled.setCursorXY(MENU_PARAMS_LEFT_OFFSET + 8, index * MENU_SELECTED_H + MENU_ITEM_PADDING_TOP);
  oled.print(*static_cast<const int*>(val));
}


bool prMicrostepResolution(const int index, const void* val, const byte valType)
{
  customLeftPadding(index);
  oled.print(GetResolutionStr(currentSettings.getMicrostepResolution()));
  return true;
}

bool prSelectDriver(const int index, const void* val, const byte valType)
{
  customLeftPadding(index);
  oled.print(GetDriverStr(currentSettings.getDriverType()));
  return true;
}

//menu func

void sensorCheck()
{
  sensor.tick(); //проверка сенсора
  int currTick = millis();
  if ((sensor.isClick() || sensor.isHold()) && 
      (currTick - prevSensMillis) < TIME_BETWEEN_SENSORE_USES)
  {
    prevSensMillis = currTick;
    Extrude();
  }
}

template<typename OledMenu>
bool MenuEncLoopInt(OledMenu& menu)
{
  if(enc1.isClick())
  {
    menu.toggleChangeSelected();
  }
  else if (enc1.isLeft())
  {
    menu.selectPrev();
  }
  else if (enc1.isRight())
  {
    menu.selectNext();
  }
  else // больше енкодеру делать нечего
  {
    return false;
  }
  return true;
}


static_assert(EEPROM_MODE == EEPROM_REGULAR_START ||
              EEPROM_MODE == EEPROM_FIRST_START ||
              EEPROM_MODE == EEPROM_AUTO_START, "EEPROM_MODE invalid value");

//init func
void init_EEPROM()
{
  #if EEPROM_MODE == EEPROM_REGULAR_START
  lastEEEPROM_elem = EEPROM.get<int>((EEPROM.begin() + DEAD_EEPROM_CELL_OFFSET), lastEEEPROM_elem);

  #elif EEPROM_MODE == EEPROM_FIRST_START || EEPROM_MODE == EEPROM_AUTO_START
  for(int i = EEPROM.begin()+ DEAD_EEPROM_CELL_OFFSET; i < EEPROM_OFFEST_BYTE_FOR_NUMERATION_SAVES; ++i)
    EEPROM.update(i,0); //в теории благодаря тому что это update не должно тратить циклы перезаписи если значения и так уже нулевые
  lastEEEPROM_elem = EEPROM.get<int>((EEPROM.begin() + DEAD_EEPROM_CELL_OFFSET), lastEEEPROM_elem);
  #endif
}

void init_main_menu(Settings* settings)
{
  main_menu.onPrintOverride(nullptr);
  main_menu.onChange(nullptr);
  main_menu.addItem(PSTR("Turn on Sensor:"), &isTurnOnSensor); //0

  main_menu.onPrintOverride(nullptr);
  main_menu.onChange(cbExtrudeOrRetract);
  main_menu.addItem(
    PSTR("Extrude/Retract:"),
    GM_N_INT(1),
    &currentExtrude,
    GM_N_INT(INT16_MIN),
    GM_N_INT(INT16_MAX)); //1

  main_menu.onPrintOverride(nullptr);
  main_menu.onChange(nullptr);
  main_menu.addItem(
    PSTR("Buffer Amount:"),  //2  количество выдавливаемого пластика за срабатывание сенсора
    GM_N_INT(BUFFER_INC), 
    &(settings->buff_amount), 
    GM_N_INT(BUFFER_MIN), 
    GM_N_INT(BUFFER_MAX));

  main_menu.onPrintOverride(prMicrostepResolution);
  main_menu.onChange(nullptr);
  main_menu.addItem(
  PSTR("Microstep:"), //3 
  GM_N_INT(MICROSTEP_RESOLUTION_INC), 
  &(settings->microstep_resolution),
  GM_N_INT(MICROSTEP_RESOLUTION_MIN), 
  GM_N_INT(MICROSTEP_RESOLUTION_MAX));

  main_menu.onPrintOverride(prSelectDriver);
  main_menu.onChange(nullptr);
  main_menu.addItem(
  PSTR("Driver:"), //4
  GM_N_INT(DRIVER_INC), 
  &(settings->driver_type),
  GM_N_INT(DRIVER_MIN), 
  GM_N_INT(DRIVER_MAX));

  main_menu.onPrintOverride(nullptr);
  main_menu.onChange(cbStepperSettings, true);
  main_menu.addItem(PSTR("Stepper Settings")); //5

  main_menu.onPrintOverride(nullptr);
  main_menu.onChange(cbSavePreset, true);
  main_menu.addItem(PSTR("Save Preset"));     //6

  main_menu.onPrintOverride(nullptr);
  main_menu.onChange(cbSelectPreset, true);
  main_menu.addItem(PSTR("Select Preset"));  //7
  
  main_menu.showMenu(true);
}

void init_stepper_menu(StepperSettings* stepper_settings)
{
  stepper_menu.onPrintOverride(nullptr);
  stepper_menu.onChange(cbBACK, true);
  stepper_menu.addItem(PSTR("BACK <-- "));

  stepper_menu.onPrintOverride(nullptr);
  stepper_menu.onChange(nullptr);
  stepper_menu.addItem(PSTR("Enable Mod"), &(stepper_settings->enable_mod));
    stepper_menu.addItem(
    PSTR("Step per mm"),  //2  соотношение шага к милиметражу выдавливаемого пластика
    GM_N_INT(STEP_PER_MM_SETUP_INC), 
    &(stepper_settings->step_per_mm), 
    GM_N_INT(STEP_PER_MM_SETUP_MIN), 
    GM_N_INT(STEP_PER_MM_SETUP_MAX));

  stepper_menu.onPrintOverride(nullptr);
  stepper_menu.onChange(nullptr);
  stepper_menu.addItem(
    PSTR("Speed"),        //3  скорость выдавливаемого пластика
    GM_N_INT(STEP_SPEED_INC), 
    &(stepper_settings->step_per_mm), 
    GM_N_INT(STEP_SPEED_MIN), 
    GM_N_INT(STEP_SPEED_MAX));
  
  stepper_menu.onPrintOverride(nullptr);
  stepper_menu.onChange(nullptr);
  stepper_menu.addItem(
        PSTR("Acceleration"),    //4  ускорение скорости выдавливаемого пластика
    GM_N_INT(STEP_ACCELERATION_INC), 
    &(stepper_settings->step_per_mm), 
    GM_N_INT(STEP_ACCELERATION_MIN), 
    GM_N_INT(STEP_ACCELERATION_MAX));   
}


void setup() {
  oled.init();
  Wire.setClock(400000L);
  oled.clear();
  oled.update();
  oled.setContrast(127);
  oled.setPower(true);
  enc1.setType(TYPE2);
  enc1.setDirection(REVERSE);

  sensor.setDebounce(50);
  sensor.setTimeout(300);
  sensor.setClickTimeout(300);
  sensor.setType(HIGH_PULL);
  sensor.setDirection(NORM);


  setDefaultSettings();
  init_EEPROM();
  init_main_menu(&currentSettings);
  init_stepper_menu(&currentStepperSettings);
}

void loop() {
  while (currentMenuType == MenuType::MainMenu)
  {
    enc1.tick();
    if(!MenuEncLoopInt(main_menu))
      sensorCheck();
  }
  
  while (currentMenuType == MenuType::StepperMenu)
  {
    enc1.tick();
    if(!MenuEncLoopInt(stepper_menu))
      sensorCheck();
  }

  while (currentMenuType == MenuType::SaveSelectionCustomMenu)
  {
    /* code */
  }
  
}
