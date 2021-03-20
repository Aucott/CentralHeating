#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <TimeLib.h>
#include <Wire.h>
#include <EEPROM.h>
#include <DS1307RTC.h> // a basic DS1307 library that returns time as a time_t
#include <elapsedMillis.h>
#include "Operation.h"
#include "EasyNextionLibrary.h" // Include EasyNextionLibrary

const int maxAllowedWrites = 80;
const int memBase = 350;

extern EasyNex myNex;
extern bool newPageLoaded;

typedef enum
{
  Sunday,
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday
} WeekDay;
extern tmElements_t tm;

bool getTime(const char *str);

bool getDate(const char *str);

#endif