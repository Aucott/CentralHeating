#include "Main.h"

elapsedMillis elapsedTime;

char buffer[25];
tmElements_t tm;
int i;

//EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
EasyNex myNex(Serial);

const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

const char *dayName[7] = {
    "Sun", "Mon", "Tue", "Wed", "Thur", "Fri",
    "Sat"};

bool newPageLoaded = false; // true when the page is first loaded ( lastCurrentPageId != currentPageId )

void setup()
{

  // get the date and time the compiler was run
  // if (getDate(__DATE__) && getTime(__TIME__)) {

  // and configure the RTC with this info
  //  RTC.write(tm);

  //}
  pinMode(BOILER_POWER, OUTPUT);
  pinMode(CH_VALVE_240V, OUTPUT);
  pinMode(HW_VALVE_240V, OUTPUT);

  for (int i = 0; i < 7; i++)
  {
    EEPROM.get(CH_EEPROM_ADDRESS + (sizeof(CH_Time) * i), CH_Times[i]);
  }

  for (int i = 0; i < 7; i++)
  {
    EEPROM.get(HW_EEPROM_ADDRESS + (sizeof(HW_Time) * i), HW_Times[i]);
  }

  TCCR2B = (TCCR2B & B11111000 | B00000001); // for PWM frequency of 31372.55 Hz
  TCCR1B = (TCCR1B & B11111000 | B00000001); // for PWM frequency of 31372.55 Hz

  myNex.begin(115200); // Begin the object with a baud rate of 9600
                       //myNex.begin(9600); // Begin the object with a baud rate of 9600

  myNex.writeStr("page 0"); // For synchronizing Nextion page in case of reset to Arduino
  delay(50);
  myNex.lastCurrentPageId = 1; // At the first run of the loop, the currentPageId and the lastCurrentPageId
                               // must have different values, due to run the function firstRefresh()

  // the function to get the time from the RTC
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
  {
    // Serial.println("Unable to sync with the RTC");
    while (1)
      ;
  }

  //setup_CH_Valve();

  // setup_HW_Valve();

  elapsedTime = 0;
}

void loop()
{

  // uint32_t currentTime = hoursToTime_t(hour()) + minutesToTime_t(minute()) + second();

  // while (elapsedTime < 1000)
  //{
  myNex.NextionListen(); // This function must be called repeatedly to response touch events}

  if (elapsedTime > 1000)
  {

    if (myNex.currentPageId == 0)
    {
      sprintf(buffer, "%s %02d:%02d:%02d", dayName[today()], hour(), minute(), second());
      myNex.writeStr("tTime.txt", buffer); // Set button b0 text to "OFF"

      sprintf(buffer, "%s%2d %s%c", "Hot Water Temp: ", 24, "\xB0", 'C');
      myNex.writeStr("tWTemp.txt", buffer);

      sprintf(buffer, "%s%2d %s%c", "Room Temp: ", 24, "\xB0", 'C');
      myNex.writeStr("tRoomTemp.txt", buffer);

      sprintf(buffer, "%s%2d %c", "Humidity: ", 80, '%');
      myNex.writeStr("tHumid.txt", buffer);

      sprintf(buffer, "%s%2d %s%c", "Outsde Temp: ", 26, "\xB0", 'C');
      myNex.writeStr("tOutTemp.txt", buffer);

      digitalWrite(HW_VALVE_240V, !digitalRead(HW_VALVE_240V));
    }

    digitalWrite(CH_VALVE_240V, !digitalRead(CH_VALVE_240V));

    elapsedTime = elapsedTime - 1000;
  }
  // put your main code here, to run repeatedly:
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3)
    return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++)
  {
    if (strcmp(Month, monthName[monthIndex]) == 0)
      break;
  }
  if (monthIndex >= 12)
    return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}