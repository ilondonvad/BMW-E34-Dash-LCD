// Disp 1 - x: 14  y: 114  w: 463  h: 138
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SimpleFTPServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <ArduinoJson.h>



#define DISP1 9
#define DISP2 16



TFT_eSPI tft = TFT_eSPI(); // Инициализация TFT дисплея
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite hideText = TFT_eSprite(&tft);
TFT_eSprite box = TFT_eSprite(&tft);

//AsyncWebServer server(80);
//FtpServer ftpSrv;

#include "vars.h"

void cs(byte disp) {
  if (disp == 1) {
    digitalWrite(DISP2, 1);
    digitalWrite(DISP1, 0);
  }
  else if (disp == 2) {
    digitalWrite(DISP1, 1);
    digitalWrite(DISP2, 0);
  }
  else if (disp == 0) {
    digitalWrite(DISP1, 1);
    digitalWrite(DISP2, 1);
  }
  else if (disp == 3) {
    digitalWrite(DISP1, 0);
    digitalWrite(DISP2, 0);
  }
}


#include "setup.h"


unsigned long t = 0;
unsigned long arT = 0;

int lastProg = 100;



void loop() {
  if (stringComplete) {
    parseInput(inputString); // Вызываем функцию парсинга
    inputString = ""; // Очищаем буфер
    stringComplete = false; // Сбрасываем флаг
  }
  while (Serial.available()) {
    char inChar = (char)Serial.read(); // Читаем символ
    inputString += inChar; // Добавляем символ в строку
    if (inChar == '\n') { // Если получили символ конца строки
      stringComplete = true;
    }
  }



  if (millis() > arT ) {
    if (lastProg < FLoad && lastProg < 101)renderLoadProgres(++lastProg);
    else if (lastProg > FLoad && lastProg > 0)renderLoadProgres(--lastProg);
    arT = millis();
  }

  if (millis() > t + 500) {
    i2c();
    drawOdo();
  }

  /*cs(3);*/


  //MDNS.update();
  //ftpSrv.handleFTP();
}

void parseInput(String data) {
  // Разбиваем строку на пары ключ-значение
  int startIndex = 0;
  while (startIndex < data.length()) {
    int endIndex = data.indexOf(';', startIndex);
    if (endIndex == -1) break; // Выходим, если больше нет пар

    String pair = data.substring(startIndex, endIndex);
    int eqIndex = pair.indexOf('=');
    if (eqIndex != -1) {
      String key = pair.substring(0, eqIndex);
      String value = pair.substring(eqIndex + 1);

      // Преобразуем ключ в переменную
      if (key == "x") x = value.toFloat();
      else if (key == "y") y = value.toFloat();
      else if (key == "a") a = value.toFloat();
      else if (key == "b") b = value.toFloat();
      else if (key == "start") start = value.toFloat();
      else if (key == "end") end = value.toFloat();
      else if (key == "cl") cl = value.toInt();
    }
    startIndex = endIndex + 1; // Переходим к следующей паре
  }

  Serial.println("Parsed values:");
  Serial.print("x = "); Serial.println(x);
  Serial.print("y = "); Serial.println(y);
  Serial.print("a = "); Serial.println(a);
  Serial.print("b = "); Serial.println(b);
  Serial.print("start = "); Serial.println(start);
  Serial.print("end = "); Serial.println(end);
  Serial.print("cl = "); Serial.println(cl);
}


void i2cSend(byte data) {
  Wire.beginTransmission(8);
  Wire.write("D");
  Wire.write(data);
  Wire.endTransmission();
}

void i2c() {
  Wire.requestFrom(8, 12);  // Запрос 12 байт данных
  if (Wire.available()) {
    odoKm = Wire.read() << 24;
    odoKm |= Wire.read() << 16;
    odoKm |= Wire.read() << 8;
    odoKm |= Wire.read();
    odoDayKm = Wire.read() << 8;
    odoDayKm |= Wire.read();
    FLoad = Wire.read() << 8;
    FLoad |= Wire.read();
    FRpm = Wire.read() << 8;
    FRpm |= Wire.read();
    FSpeed = Wire.read() << 8;
    FSpeed |= Wire.read();
  }
}


void drawSpeedLimit() {
  cs(1);
  tft.fillRect(0, 170, 480, 100, 0x9800);
  tft.fillCircle(250, 210, 40, TFT_RED);
  tft.fillCircle(250, 210, 32, TFT_WHITE);
  tft.loadFont("NotoSansBold36", LittleFS);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(String(spdLimit), 250, 215);
  tft.unloadFont();
  cs(0);
}

void drawSpeed() {
  cs(1);
  if (spd > spdLimit && spdLimitFlag == 0) {
    spdLimitColor = TFT_RED;
    //tft.fillScreen(spdLimitColor);
    tft.fillRect(14, 114, 463, 150, spdLimitColor);
    spdLimitFlag = 1;
  }
  else if (spd < spdLimit && spdLimitFlag == 1) {
    spdLimitColor = TFT_BLACK;
    tft.fillRect(14, 114, 463, 150, spdLimitColor);
    spdLimitFlag = 0;
  }
  else if (spd > spdLimit)spdLimitColor = TFT_RED;
  else spdLimitColor = TFT_BLACK;

  hideText.createSprite(120, 60);
  hideText.fillSprite(spdLimitColor);
  hideText.loadFont("Unicode-Test-72", LittleFS);
  hideText.setTextColor(TFT_WHITE, spdLimitColor);  // Устанавливаем цвет текста белым
  hideText.setTextDatum(TR_DATUM);
  hideText.drawString(String(spd), 120, 0);
  hideText.pushSprite(180, 150);
  hideText.deleteSprite();
  hideText.unloadFont();
  tft.loadFont("Latin-Hiragana-24", LittleFS);
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(TFT_WHITE, spdLimitColor);
  tft.drawString("SPEED", 165, 182);
  tft.drawString("KM/H", 385, 182);
  tft.unloadFont();
  cs(0);
}


void drawTempWatch() {
  cs(1);
  tft.loadFont("Latin-Hiragana-24", LittleFS);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Устанавливаем цвет текста белым
  tft.setTextPadding(50);
  tft.drawString("-22C", 200, 180);
  tft.drawString("12:22", 350, 180);
  tft.drawString("03.11", 350, 210);
  tft.unloadFont();
  cs(0);
}


void drawOdo() {
  uint32_t od = filterData(odoKm);
  uint16_t odD = filterData2(odoDayKm);
  cs(1);
  tft.loadFont("NotoSansBold36", LittleFS);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Устанавливаем цвет текста белым
  tft.setTextPadding(50);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(String(od), 200, 130, TFT_BLACK);
  tft.drawString(String(odD), 400, 130);
  tft.unloadFont();
  tft.setTextPadding(0);
  tft.loadFont("NotoSansBold15", LittleFS);
  tft.drawString("KM", 230, 146);
  tft.drawString("KM", 430, 146);
  tft.drawString("TRIP", 330, 146);
  tft.unloadFont();
  cs(0);
}


void drawHello() {
  cs(1);
  tft.loadFont("NotoSansBold36", LittleFS);
  tft.setTextColor(0x07df, TFT_BLACK);  // Устанавливаем цвет текста белым
  tft.drawString("Welcome to BMW", 90, 150);
  tft.setTextColor(0x381f, TFT_BLACK);
  tft.drawString("Airlines", 180, 200);
  tft.unloadFont();
  delay(1500);
  tft.fillScreen(TFT_BLACK);
  cs(0);
}


void drawEngTemp() {
  /*File configFile = LittleFS.open("config.json", "r");
    if (!configFile) {
    Serial.println("Failed to open config file");
    return;
    }

    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, configFile);
    if (error) {
    Serial.print("Failed to parse config file: ");
    Serial.println(error.c_str());
    return;
    }

    float x = doc["x"];
    float y = doc["y"];
    float a = doc["a"];
    float b = doc["b"];
    float start = doc["start"];
    float end = doc["end"];
    uint32_t cl = doc["cl"];*/
  //configFile.close();
  cs(2);

  tft.drawSmoothArc(255, -75, 310, 312, 330, 30, TFT_WHITE, TFT_WHITE);
  tft.drawSmoothArc(255, -100, 310, 331, 20, 28, 27871, 27871);
  tft.drawSmoothArc(255, -100, 310, 331, 332, 340, 63488, 63488);
  tft.drawSmoothArc(255, -80, 322, 336, 330, 30, TFT_WHITE, TFT_WHITE);

  tft.drawWideLine(105.51, 171.57, 93.45, 193.47, 2, TFT_WHITE, TFT_WHITE);
  tft.drawWideLine(189.10, 202.92, 183.79, 227.34, 2, TFT_WHITE, TFT_WHITE);
  tft.drawWideLine(257.11, 209.99, 257.28, 234.99, 3, TFT_WHITE, TFT_WHITE);
  tft.drawWideLine(319.86, 203.14, 325.09, 227.09, 2, TFT_WHITE, TFT_WHITE);
  tft.drawWideLine(404.49, 171.57, 416.55, 193.48, 2, TFT_WHITE, TFT_WHITE);

  tft.loadFont("Latin-Hiragana-24", LittleFS);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Устанавливаем цвет текста белым
  tft.drawString("TEMP", 230, 135);
  tft.unloadFont();
  cs(0);
}

void drawEngLoad() {
  cs(2);
  //tft.fillScreen(TFT_BLACK);
#define initX 260
#define initY -65
#define maxRad 320

  TFT_eSprite text = TFT_eSprite(&tft);
  text.fillSprite(TFT_BLACK);
  text.loadFont("Latin-Hiragana-24", LittleFS);
  text.setTextColor(TFT_WHITE);  // Устанавливаем цвет текста белым

  text.createSprite(50, 20);
  tft.setPivot(initX + 190, initY + 220);
  text.drawString("100", 0, 0);
  text.pushRotated(-30);

  text.fillSprite(TFT_BLACK);
  tft.setPivot(initX - 160, initY + 235);
  text.drawString("0", 0, 0);
  text.pushRotated(30);

  text.fillSprite(TFT_BLACK);
  tft.setPivot(0, 0);
  text.drawString("50", 0, 0);
  text.pushSprite(initX - 5, initY + 275);

  text.deleteSprite();
  text.createSprite(130, 20);
  text.fillSprite(TFT_BLACK);
  tft.setPivot(0, 0);
  text.drawString("LOAD", 0, 0);
  text.pushSprite(initX - 25, initY + 200);

  text.deleteSprite();
  text.unloadFont();

  for (float i = 0; i < 85; i += 6) {
    float x1 = initX + cos((135 - i) * 0.017) * (maxRad - 15);
    float y1 = initY + sin((135 - i) * 0.017) * (maxRad - 15);
    float x2 = initX + cos((135 - i) * 0.017) * maxRad;
    float y2 = initY + sin((135 - i) * 0.017) * maxRad;
    tft.drawWideLine(x1, y1, x2, y2, 3, TFT_RED, TFT_RED);
  }
  cs(0);
}


void renderLoadProgres(int input) {
  byte black = 0;
  byte bl = 0;
  cs(2);
  box.createSprite(23, 15);
  int mapInput = (input * 14 / 100);
  int ang = 0;
  for (byte i = 0; i < 14; i++) {
    if (mapInput > i && mapInput != 0) {
      box.fillSprite(0xFF7B00);
      bl = 1;
    }
    else {
      box.fillSprite(TFT_BLACK);
      black = 1;
    }
    ang = (132 - i * 6);
    float x1 = initX + cos(ang * 0.017) * (maxRad - 6);
    float y1 = initY + sin(ang * 0.017) * (maxRad - 6);
    tft.setPivot(x1, y1);
    box.pushRotated(90 + ang);
    if (black == 1)continue;
    else if (bl == 1)continue;
    yield();
  }
  box.deleteSprite();
  cs(0);
}



uint32_t filterData(long newValue)
{
  if (!isInitialized)
  {
    lastValue = newValue;
    isInitialized = true;
    return newValue;
  }
  if (abs(newValue - lastValue) <= MAX_DIFF)
  {
    lastValue = newValue; // Обновляем корректное значение
  }
  return lastValue; // Возвращаем последнее корректное значение
}

uint16_t filterData2(uint16_t newValue)
{
  if (!isInitialized2)
  {
    lastValue2 = newValue;
    isInitialized2 = true;
    return newValue;
  }
  if (abs(newValue - lastValue2) <= MAX_DIFF)
  {
    lastValue2 = newValue; // Обновляем корректное значение
  }
  return lastValue2; // Возвращаем последнее корректное значение
}
