#include <Wire.h>
#include <avr/wdt.h>
#include "vars.h"

void setup()
{
  wdt_enable(WDTO_1S);
  Wire.begin(8);                // Инициализация I2C в режиме слейва с адресом 8
  Wire.onReceive(receiveEvent); // Обработчик приема данных
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
  pinMode(CLOCK, INPUT);
  pinMode(LOAD, INPUT);
  pinMode(DATA, INPUT);
  pinMode(LIGHT, 1);
  pinMode(ENG_LOAD, INPUT);
  pinMode(ENG_RPM, INPUT);
  pinMode(CAR_SPEED, INPUT);
  EICRA |= (1 << ISC00) | (1 << ISC01);
  EICRA |= (1 << ISC10) | (1 << ISC11);
  // EIMSK |= (1 << INT0) | (1 << INT1);
  EIMSK |= (1 << INT1);

  DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2)); // Настройка PB0, PB1, PB2 как входов
  PORTB |= ((1 << PB0) | (1 << PB1) | (1 << PB2)); // Включаем подтяжку

  // Включение прерываний для PCINT0-7
  PCICR |= (1 << PCIE0); // Включаем прерывания для группы PCINT[7:0]
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2);

  sei();
}

byte light = 0;
byte lightSet = 0;
unsigned long timer1 = 0;

void loop()
{
  wdt_reset();
  
  if (millis() % 50 == 0 && light < lightSet)
  {
    analogWrite(LIGHT, light);
    light++;
  }
  else if (millis() % 50 == 0 && light > lightSet)
  {
    analogWrite(LIGHT, light);
    light--;
  }
  FLoad = (period[0] > 0) ? 1000000UL / period[0] : 0;
  FRpm = (period[1] > 0) ? 1000000UL / period[1] : 0;
  FSpeed = (period[2] > 0) ? 1000000UL / period[2] : 0;
}
void requestEvent()
{
  Wire.write((byte)(odo >> 24)); // Отправка старшего байта
  Wire.write((byte)(odo >> 16)); // Отправка следующего байта
  Wire.write((byte)(odo >> 8));  // Отправка следующего байта
  Wire.write((byte)odo);
  Wire.write((byte)(dayOdo >> 8));
  Wire.write((byte)dayOdo);

  Wire.write((byte)(FLoad >> 8));
  Wire.write((byte)FLoad);

  Wire.write((byte)(FRpm >> 8));
  Wire.write((byte)FRpm);

  Wire.write((byte)(FSpeed >> 8));
  Wire.write((byte)FSpeed);
}

void receiveEvent(int howMany)
{
  if (howMany > 0)
  {
    if (Wire.read() == 'D')
    {
      lightSet = Wire.read();
    }
  }
}

#include "calc_odo.h"
#include "calc_freq.h"
