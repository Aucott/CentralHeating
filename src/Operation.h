#ifndef OPERATION_H
#define OPERATION_H

#include <Arduino.h>

#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h> // a basic DS1307 library that returns time as a time_t
#include <elapsedMillis.h>

#define CH_EEPROM_ADDRESS 0
#define HW_EEPROM_ADDRESS (sizeof(CH_Time)*7)

#define REVERSE 0
#define FORWARD 1

#define ERROR_NONE 1
#define ERROR_OVER_CURRENT 2
#define ERROR_OPEN_CIRCUIT 3
#define ERROR_VALVE_JAM 4

#define BOILER_POWER 2
#define CH_VALVE_240V 8
#define HW_VALVE_240V 4
#define HW_FIN 11
#define HW_RIN 3
#define CH_FIN 9
#define CH_RIN 10

#define HW_VALVE_OPEN_240V 5
#define CH_VALVE_OPEN_240V 6

#define HW_CURRENT_ADC 14
#define CH_CURRENT_ADC 15
#define WATER_TEMP_ADC 16
#define REF_VOLT_ADC A7

#define NOMINAL_VALVE_TIME 3500

#define I225mA (uint16_t)((1023 / 5.0) * 2.25)
#define I200mA (uint16_t)((1023 / 5.0) * 2.0)
#define I100mA (uint16_t)((1023 / 5.0) * 1.0)
#define I005mA (uint16_t)((1023 / 5.0) * 0.05)

// function prototypes
bool checkCH(uint32_t tsec);
bool checkHW(uint32_t tsec);
void calcOffset(void);
uint16_t getHW_Current(void);
uint16_t getCH_Current(void);
uint8_t setup_CH_Valve(void);
uint8_t setup_HW_Valve(void);
uint8_t open_HW_Valve(void);

extern struct CH_Struct CH_Properties;
extern struct HW_Struct HW_Properties;


extern  struct CH_Time CH_Times[7];
extern  struct HW_Time HW_Times[7];


extern uint16_t adcOffset;

struct CH_Time
{
    uint32_t Start1;
    uint32_t End1;
    uint32_t Start2;
    uint32_t End2;
    uint32_t Start3;
    uint32_t End3;
} ;

struct HW_Time
{
    uint32_t Start1;
    uint32_t End1;
    uint32_t Start2;
    uint32_t End2;
    uint32_t Start3;
    uint32_t End3;
};

struct CH_Struct
{
    uint16_t midPositionTime;
    uint8_t valveOpen;
    uint8_t boilerRunning;
};
struct HW_Struct
{    
    uint16_t midPositionTime;
    uint8_t valveOpen;
    uint8_t boilerRunning;
};

#endif