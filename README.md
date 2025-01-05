# BMW-E34-Dash-LCD
How to replace old displays with new tft
![photo_2025-01-05_13-09-00](https://github.com/user-attachments/assets/6eabbf0b-328c-4ef3-be0f-685107e1c10f)

## PCB плата:


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

