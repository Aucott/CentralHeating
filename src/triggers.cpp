
#include "triggers.h"

void trigger0()
{
    // To call this void send from Nextion's component's Event:  printh 23 02 54 00
    // In this exmaple, we send this command from the Release Event of b0 button (see the HMI of this example)
    // You can send  the same `printh` command, to call the same function, from more than one component, depending on your needs

    digitalWrite(CH_VALVE_240V, !digitalRead(CH_VALVE_240V));
}
//---------------------------------------------------------------------------------
//update LCD CH schedule times IN display
void trigger1()
{

    int index = myNex.readNumber("va0.val");

    myNex.writeNum("n0.val", index);
    myNex.writeNum("nSH1.val", hour(CH_Times[index].Start1));
    myNex.writeNum("nSM1.val", minute(CH_Times[index].Start1));
    myNex.writeNum("nEH1.val", hour(CH_Times[index].End1));
    myNex.writeNum("nEM1.val", minute(CH_Times[index].End1));

    myNex.writeNum("nSH2.val", hour(CH_Times[index].Start2));
    myNex.writeNum("nSM2.val", minute(CH_Times[index].Start2));
    myNex.writeNum("nEH2.val", hour(CH_Times[index].End2));
    myNex.writeNum("nEM2.val", minute(CH_Times[index].End2));

    myNex.writeNum("nSH3.val", hour(CH_Times[index].Start3));
    myNex.writeNum("nSM3.val", minute(CH_Times[index].Start3));
    myNex.writeNum("nEH3.val", hour(CH_Times[index].End3));
    myNex.writeNum("nEM3.val", minute(CH_Times[index].End3));
}
//---------------------------------------------------------------------------------
//Save page schedule values into eeprom
void trigger2()
{

    int index = myNex.readNumber("va0.val");

    CH_Times[index].Start1 = hoursToTime_t(myNex.readNumber("nSH1.val")) + minutesToTime_t(myNex.readNumber("nSM1.val"));
    CH_Times[index].End1 = hoursToTime_t(myNex.readNumber("nEH1.val")) + minutesToTime_t(myNex.readNumber("nEM1.val"));

    CH_Times[index].Start2 = hoursToTime_t(myNex.readNumber("nSH2.val")) + minutesToTime_t(myNex.readNumber("nSM2.val"));
    CH_Times[index].End2 = hoursToTime_t(myNex.readNumber("nEH2.val")) + minutesToTime_t(myNex.readNumber("nEM2.val"));

    CH_Times[index].Start3 = hoursToTime_t(myNex.readNumber("nSH3.val")) + minutesToTime_t(myNex.readNumber("nSM3.val"));
    CH_Times[index].End3 = hoursToTime_t(myNex.readNumber("nEH3.val")) + minutesToTime_t(myNex.readNumber("nEM3.val"));

    EEPROM.put(CH_EEPROM_ADDRESS + (sizeof(CH_Time) * index), CH_Times[index]); //Save to eprom

    //myNex.writeNum("n0.val", hoursToTime_t(myNex.readNumber("nSH1.val"))+minutesToTime_t(myNex.readNumber("nSM1.val")));
}

void trigger3()
{
    int indexWrite;
    int indexRead = myNex.readNumber("va0.val");
    indexWrite = indexRead + 1;

    if (indexWrite > 6)
        indexWrite = 0;

    CH_Times[indexWrite].Start1 = CH_Times[indexRead].Start1;
    CH_Times[indexWrite].End1 = CH_Times[indexRead].End1;
    CH_Times[indexWrite].Start2 = CH_Times[indexRead].Start2;
    CH_Times[indexWrite].End2 = CH_Times[indexRead].End2;
    CH_Times[indexWrite].Start3 = CH_Times[indexRead].Start3;
    CH_Times[indexWrite].End3 = CH_Times[indexRead].End3;

    EEPROM.put(CH_EEPROM_ADDRESS + (sizeof(CH_Time) * indexWrite), CH_Times[indexWrite]); //Save to eprom
}
//---------------------------------------------------------------------------------
//Save page schedule values into eeprom
void trigger4()
{
    int indexWrite;
    int indexRead = myNex.readNumber("va0.val");
    indexWrite = indexRead - 1;

    if (indexWrite < 0)
        indexWrite = 6;

    CH_Times[indexWrite].Start1 = CH_Times[indexRead].Start1;
    CH_Times[indexWrite].End1 = CH_Times[indexRead].End1;
    CH_Times[indexWrite].Start2 = CH_Times[indexRead].Start2;
    CH_Times[indexWrite].End2 = CH_Times[indexRead].End2;
    CH_Times[indexWrite].Start3 = CH_Times[indexRead].Start3;
    CH_Times[indexWrite].End3 = CH_Times[indexRead].End3;

    EEPROM.put(CH_EEPROM_ADDRESS + (sizeof(CH_Time) * indexWrite), CH_Times[indexWrite]); //Save to eeprom
}
//---------------------------------------------------------------------------------
//update LCD HW schedule times in display
void trigger5()
{

    int index = myNex.readNumber("va0.val");

    myNex.writeNum("n0.val", index);
    myNex.writeNum("nSH1.val", hour(HW_Times[index].Start1));
    myNex.writeNum("nSM1.val", minute(HW_Times[index].Start1));
    myNex.writeNum("nEH1.val", hour(HW_Times[index].End1));
    myNex.writeNum("nEM1.val", minute(HW_Times[index].End1));

    myNex.writeNum("nSH2.val", hour(HW_Times[index].Start2));
    myNex.writeNum("nSM2.val", minute(HW_Times[index].Start2));
    myNex.writeNum("nEH2.val", hour(HW_Times[index].End2));
    myNex.writeNum("nEM2.val", minute(HW_Times[index].End2));

    myNex.writeNum("nSH3.val", hour(HW_Times[index].Start3));
    myNex.writeNum("nSM3.val", minute(HW_Times[index].Start3));
    myNex.writeNum("nEH3.val", hour(HW_Times[index].End3));
    myNex.writeNum("nEM3.val", minute(HW_Times[index].End3));
}
//---------------------------------------------------------------------------------
//Save HW page schedule values into eeprom
void trigger6()
{

    int index = myNex.readNumber("va0.val");

    HW_Times[index].Start1 = hoursToTime_t(myNex.readNumber("nSH1.val")) + minutesToTime_t(myNex.readNumber("nSM1.val"));
    HW_Times[index].End1 = hoursToTime_t(myNex.readNumber("nEH1.val")) + minutesToTime_t(myNex.readNumber("nEM1.val"));

    HW_Times[index].Start2 = hoursToTime_t(myNex.readNumber("nSH2.val")) + minutesToTime_t(myNex.readNumber("nSM2.val"));
    HW_Times[index].End2 = hoursToTime_t(myNex.readNumber("nEH2.val")) + minutesToTime_t(myNex.readNumber("nEM2.val"));

    HW_Times[index].Start3 = hoursToTime_t(myNex.readNumber("nSH3.val")) + minutesToTime_t(myNex.readNumber("nSM3.val"));
    HW_Times[index].End3 = hoursToTime_t(myNex.readNumber("nEH3.val")) + minutesToTime_t(myNex.readNumber("nEM3.val"));

    EEPROM.put(HW_EEPROM_ADDRESS + (sizeof(HW_Time) * index), HW_Times[index]); //Save to Eeprom
}
//--------------------------------------------------------------------------------------------------------
//Trigger function forward copy values into eeprom
void trigger7()
{
    int indexWrite;
    int indexRead = myNex.readNumber("va0.val");
    indexWrite = indexRead + 1;

    if (indexWrite > 6)
        indexWrite = 0;

    HW_Times[indexWrite].Start1 = HW_Times[indexRead].Start1;
    HW_Times[indexWrite].End1 = HW_Times[indexRead].End1;
    HW_Times[indexWrite].Start2 = HW_Times[indexRead].Start2;
    HW_Times[indexWrite].End2 = HW_Times[indexRead].End2;
    HW_Times[indexWrite].Start3 = HW_Times[indexRead].Start3;
    HW_Times[indexWrite].End3 = HW_Times[indexRead].End3;

    EEPROM.put(HW_EEPROM_ADDRESS + (sizeof(HW_Time) * indexWrite), HW_Times[indexWrite]); //Save to eprom
}
//--------------------------------------------------------------------------------------------------------
//Trigger function backward copy values into eeprom
void trigger8()
{
    int indexWrite;
    int indexRead = myNex.readNumber("va0.val");
    indexWrite = indexRead - 1;

    if (indexWrite < 0)
        indexWrite = 6;

    HW_Times[indexWrite].Start1 = HW_Times[indexRead].Start1;
    HW_Times[indexWrite].End1 = HW_Times[indexRead].End1;
    HW_Times[indexWrite].Start2 = HW_Times[indexRead].Start2;
    HW_Times[indexWrite].End2 = HW_Times[indexRead].End2;
    HW_Times[indexWrite].Start3 = HW_Times[indexRead].Start3;
    HW_Times[indexWrite].End3 = HW_Times[indexRead].End3;

    EEPROM.put(HW_EEPROM_ADDRESS + (sizeof(HW_Time) * indexWrite), HW_Times[indexWrite]); //Save to eeprom
}

//update LCD CH schedule times IN display
void trigger9()
{
    if (myNex.currentPageId != 4)
        return;
    myNex.writeNum("nHour.val", hour());
    myNex.writeNum("nMinute.val", minute());
    myNex.writeNum("nSecond.val", 0);
    myNex.writeNum("nDay.val", day());
    myNex.writeNum("nMonth.val", month());
    myNex.writeNum("nYear.val", year());
}

void trigger10()
{
    if (myNex.currentPageId != 4)
        return;
    myNex.writeNum("nHour.val", hour());
    myNex.writeNum("nMinute.val", minute());
    myNex.writeNum("nSecond.val", 0);
    myNex.writeNum("nDay.val", day());
    myNex.writeNum("nMonth.val", month());
    myNex.writeNum("nYear.val", year());
}