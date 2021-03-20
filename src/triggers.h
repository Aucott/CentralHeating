#ifndef TRIGGERS_H
#define TRIGGERS_H



#include <Arduino.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h> // a basic DS1307 library that returns time as a time_t
#include <elapsedMillis.h>
#include "Operation.h"
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary
#include "Main.h"

void firstRefresh();


#endif