# BMW-E34-Dash-LCD
How to replace old displays with new tft
![photo_2025-01-05_13-09-00](https://github.com/user-attachments/assets/6eabbf0b-328c-4ef3-be0f-685107e1c10f)

## PCB плата:
Файл гербер в репозитории
![image](https://github.com/user-attachments/assets/a7b3c5ec-3ce6-49d3-87c8-d513699c20ca)

## Библиотека для работы с диплеем:
[https://github.com/ilondonvad/MSM5265-GET-DATA](https://github.com/Bodmer/TFT_eSPI)

## Важная настройка файлов конфигурации eSPI TFT
В папке расположения самой библиотеки например %пользователь%/Documents\Arduino\libraries\TFT_eSPI
Файл User_Setup.h раскоментировать строку
```cpp
....
//   run without the need to make any more changes for a particular hardware setup!
//   Note that some sketches are designed for a particular TFT pixel width/height

// User defined information reported by "Read_User_Setup" test & diagnostics example
#define USER_SETUP_INFO "User_Setup"

// Define to disable all #warnings in library (can be put in User_Setup_Select.h)
//#define DISABLE_ALL_LIBRARY_WARNINGS
....
```

Файл User_Setup_Select.h найти строку с вашим контролером и раскоментировать строку настройки
```cpp
....
//#include <User_Setups/Setup19_RM68140_Parallel.h>	 // Setup file configured for RM68140 with parallel bus

#include <User_Setups/Setup20_ILI9488.h>           // Setup file for ESP8266 and ILI9488 SPI bus TFT
//#include <User_Setups/Setup21_ILI9488.h>           // Setup file for ESP32 and ILI9488 SPI bus TFT
....
```

Файл User_Setups/Setup20_ILI9488.h
```cpp
....
// See SetupX_Template.h for all options available
#define USER_SETUP_ID 20

#define ILI9488_DRIVER


// For NodeMCU - use pin numbers in the form PIN_Dx where Dx is the NodeMCU pin designation
//#define TFT_CS   PIN_D8  // Chip select control pin D8
#define TFT_DC   PIN_D4  // Data Command control pin
#define TFT_RST  PIN_D3  // Reset pin (could connect to NodeMCU RST, see next line)
//#define TFT_RST  -1  // Set TFT_RST to -1 if the display RESET is connected to NodeMCU RST or 3.3V


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT


// #define SPI_FREQUENCY  20000000
//#define SPI_FREQUENCY  27000000
 //#define SPI_FREQUENCY  40000000
 #define SPI_FREQUENCY  40000000


//#define SPI_TOUCH_FREQUENCY  2500000


// #define SUPPORT_TRANSACTIONS
....
```

## Используемые контроллеры:
1. ESP8266
2. Atmega328
3. CH340

ESP используется для отрисовки на дисплеях, Atmega берет на себя все функции по считыванию сигналов и отправку их по I2C на ESP, так же она управляет подсветкой.
Даная связка поддерживает настолько простые изображения и анимации на сколько позволят ресурсы ESP.

## Получение данных и сигналов
Как парсить пробег с приборной панели описано [в этом репозитории](https://github.com/ilondonvad/MSM5265-GET-DATA)

Сигналы скорости, нагрузки двигателя и обороты можно считать с буфера на плате, тип данных частота.
![photo_2025-01-05_13-03-16](https://github.com/user-attachments/assets/d2e33971-0b3e-40c4-bd98-a232a3875129)
![BUFER](https://github.com/user-attachments/assets/0bd7676a-be41-40c8-8fdd-b2c1501a7488)

