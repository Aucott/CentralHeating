#include "Operation.h"

CH_Struct CH_Properties;
HW_Struct HW_Properties;

  CH_Time CH_Times[7];
   HW_Time HW_Times[7];


uint16_t adcOffset;
//--------------------------------------------------------------------------------------
bool checkCH(uint32_t tsec)
{
    bool returnVal = false;

    if ((tsec > CH_Times[today()].Start1) && (tsec < CH_Times[today()].End1))
    {
        Serial.println("BOILER ON");
        CH_Properties.boilerRunning = true;

        digitalWrite(BOILER_POWER, HIGH);
        returnVal = true;
    }
    else
    {
        Serial.println("BOILER OFF");
        digitalWrite(BOILER_POWER, LOW);

        CH_Properties.boilerRunning = false;
        returnVal = false;
    }

    return returnVal;
}
//--------------------------------------------------------------------------------------
bool checkHW(uint32_t tsec)
{

    bool returnVal = false;

    if ((tsec > HW_Times[today()].Start1) && (tsec < HW_Times[today()].End1))
    {
        Serial.println("ON");
        digitalWrite(BOILER_POWER, HIGH);
        HW_Properties.boilerRunning = true;
        returnVal = true;
    }
    else
    {
        Serial.println("OFF");
        digitalWrite(BOILER_POWER, LOW);
        HW_Properties.boilerRunning = false;
        returnVal = false;
    }

    return returnVal;
}
//--------------------------------------------------------------------------------------
// works out the time taken to reach the valves mid point open position
uint8_t setup_CH_Valve()
{
    elapsedMillis elapsedTime;

    calcOffset(); // Read and store virtual ground voltage

    uint8_t PWM_Value = 0xD7; //soft start 85% duty

    elapsedTime = 0;

    //Move to A to B position
    analogWrite(CH_RIN, 0x00);
    analogWrite(CH_FIN, PWM_Value);
    delay(60); // wait for capacitor to charge as to not trip over current

    while (1)
    {
        uint16_t current = getCH_Current();

        if (current >= I225mA)
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(CH_FIN, PWM_Value); // overcurrent reduce pwm
        }

        if (current <= I005mA) // current  below 5mA  continue to next stage
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            break;
        }

        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(CH_FIN, PWM_Value); // overcurrent reduce pwm
        }

        if (elapsedTime > 5000) // valve timed out
        {

            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            return ERROR_VALVE_JAM;
        }
    }

    delay(1000);
    //Move to A to B position
    uint32_t timeTaken;
    elapsedTime = 0;
    PWM_Value = 0xD7;
    analogWrite(CH_RIN, PWM_Value); // turn off
    analogWrite(CH_FIN, 0x00);
    delay(60); // wait for capacitor to charge as to not trip over current

    while (1)
    {
        uint16_t current = getCH_Current();

        if (current >= I225mA)
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(CH_RIN, PWM_Value); // overcurrent reduce pwm
        }
        if (current <= I005mA) // current  below 5mA  continue to next stage
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            timeTaken = elapsedTime / 2;
            break;
        }

        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(CH_RIN, PWM_Value); //Increase voltage
        }

        if (elapsedTime > 12000) // valve timed out
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            return ERROR_VALVE_JAM;
        }
    }

    if ((timeTaken > (NOMINAL_VALVE_TIME - 600)) || (timeTaken < (NOMINAL_VALVE_TIME + 600)))
    {
        CH_Properties.midPositionTime = timeTaken;
    }
    else
    {
        return ERROR_VALVE_JAM;
    }

    return 0;
}

//--------------------------------------------------------------------------------------
// works out the time taken to reach the valves mid point open position
uint8_t setup_HW_Valve()
{
    elapsedMillis elapsedTime;

    calcOffset(); // Read and store virtual ground voltage

    uint8_t PWM_Value = 0xD7; //soft start 85% duty

    elapsedTime = 0;

    //Move to A to B position
    analogWrite(HW_RIN, 0x00);
    analogWrite(HW_FIN, PWM_Value);
    delay(60); // wait for capacitor to charge to not trip over current

    while (1)
    {
        uint16_t current = getCH_Current();

        if (current >= I225mA)
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(HW_FIN, PWM_Value); // overcurrent reduce pwm
        }
        if (current <= I005mA) // current  below 5mA  continue to next stage
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            break;
        }
        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(HW_FIN, PWM_Value); //Increase voltage
        }

        if (elapsedTime > 5000) // valve timed out
        {

            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            return ERROR_VALVE_JAM;
        }
    }

    delay(1000);
    //Move to A to B position
    uint32_t timeTaken;
    elapsedTime = 0;
    PWM_Value = 0xD7;
    analogWrite(HW_RIN, PWM_Value); // turn off
    analogWrite(HW_FIN, 0x00);
    delay(60); // wait for capacitor to charge as to not trip over current

    while (1)
    {
        uint16_t current = getHW_Current();

        if (current >= I225mA)
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(HW_RIN, PWM_Value); // overcurrent reduce pwm
        }
        if (current <= I005mA) // current  below 5mA  continue to next stage
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            timeTaken = elapsedTime / 2;
            break;
        }
        if (elapsedTime > 12000) // valve timed out
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            return ERROR_VALVE_JAM;
        }
    }

    if ((timeTaken > (NOMINAL_VALVE_TIME - 600)) || (timeTaken < (NOMINAL_VALVE_TIME + 600)))
    {
        HW_Properties.midPositionTime = timeTaken;
    }
    else
    {
        return ERROR_VALVE_JAM;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
//Average over 16 readings
void calcOffset(void)
{
    uint32_t Offset = 0;

    for (int i = 0; i < 16; i++)
    {
        Offset += analogRead(REF_VOLT_ADC);
        delayMicroseconds(10);
    }
    adcOffset = Offset / 16;
}
//--------------------------------------------------------------------------------------
uint16_t getCH_Current(void)
{
    uint16_t adcVal = analogRead(CH_CURRENT_ADC);

    if (adcVal == adcOffset)
        return 0;

    if (adcVal > adcOffset)
        return adcVal - adcOffset;

    if (adcOffset > adcVal)
        return adcOffset - adcVal;
    return 0;
}
//--------------------------------------------------------------------------------------
uint16_t getHW_Current(void)
{
    uint16_t adcVal = analogRead(HW_CURRENT_ADC);

    if (adcVal == adcOffset)
        return 0;

    if (adcVal > adcOffset)
        return adcVal - adcOffset;

    if (adcOffset > adcVal)
        return adcOffset - adcVal;

    return 0;
}

//--------------------------------------------------------------------------------------
// Open valve to mid open position based on the previosly calculated time
uint8_t open_HW_Valve(void)
{
    elapsedMillis elapsedTime;
    uint16_t openTime = HW_Properties.midPositionTime;

    calcOffset(); // Read and store virtual ground voltage

    uint8_t PWM_Value = 0xD7; //soft start 85% duty

    elapsedTime = 0;

    //Move to A to B position
    analogWrite(HW_RIN, 0x00);
    analogWrite(HW_FIN, PWM_Value);
    delay(60);        // wait for capacitor to charge to not trip over current
    PWM_Value = 0xFF; //soft start 85% duty
    while (elapsedTime < openTime)
    {
        uint16_t current = getHW_Current();

        if (current >= I225mA)
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(HW_FIN, PWM_Value); // overcurrent reduce pwm
        }

        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(HW_FIN, PWM_Value); // current low Increase voltage
        }
    }

    analogWrite(HW_RIN, 0x00); // turn off
    analogWrite(HW_FIN, 0x00);

    return ERROR_NONE;
}
//--------------------------------------------------------------------------------------
// Open valve to mid open position based on the previosly calculated time
uint8_t open_CH_Valve(void)
{
    elapsedMillis elapsedTime;
    uint16_t openTime = CH_Properties.midPositionTime;

    calcOffset(); // Read and store virtual ground voltage

    uint8_t PWM_Value = 0xD7; //soft start 85% duty

    elapsedTime = 0;

    //Move to A to B position
    analogWrite(CH_RIN, 0x00);
    analogWrite(CH_FIN, PWM_Value);
    delay(60);        // wait for capacitor to charge to not trip over current
    PWM_Value = 0xFF; //soft start 85% duty
    while (elapsedTime < openTime)
    {
        uint16_t current = getCH_Current();

        if (current >= I225mA)
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(CH_FIN, PWM_Value); // overcurrent reduce pwm
        }

        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(CH_FIN, PWM_Value); // current low Increase voltage
        }
    }

    analogWrite(CH_RIN, 0x00); // turn off
    analogWrite(CH_FIN, 0x00);
    CH_Properties.valveOpen = true;

    return ERROR_NONE;
}

//--------------------------------------------------------------------------------------
// Open valve to mid open position based on the previosly calculated time
uint8_t close_HW_Valve(void)
{
    elapsedMillis elapsedTime;
    uint16_t current = 0;
    uint16_t maxCloseTime = 6000; //6 seconds

    calcOffset(); // Read and store virtual ground voltage for current measurement

    uint8_t PWM_Value = 0xD7; //soft start 85% duty

    elapsedTime = 0;

    //Move to B to A position
    analogWrite(HW_RIN, PWM_Value);
    analogWrite(HW_FIN, 0x00);
    delay(60);        // wait for capacitor to charge to not trip over current
    PWM_Value = 0xFF; //soft start 85% duty

    while (elapsedTime < maxCloseTime)
    {
        current = getHW_Current();

        if (current >= I225mA)
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(HW_FIN, PWM_Value); // overcurrent reduce pwm
        }

        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(HW_FIN, PWM_Value); // current low Increase voltage
        }

        if (current <= I005mA) // current  below 5mA  continue to next stage
        {
            analogWrite(HW_RIN, 0x00); // turn off
            analogWrite(HW_FIN, 0x00);
            if (elapsedTime < 1800)
            {
                return ERROR_VALVE_JAM;
            }
            HW_Properties.valveOpen = false;
            return ERROR_NONE;
        }
    }

    analogWrite(HW_RIN, 0x00); // turn off
    analogWrite(HW_FIN, 0x00);

    return ERROR_VALVE_JAM;
}

//--------------------------------------------------------------------------------------
// Open valve to mid open position based on the previosly calculated time
uint8_t close_CH_Valve(void)
{
    elapsedMillis elapsedTime;
    uint16_t current = 0;
    uint16_t maxCloseTime = 6000; //6 seconds

    calcOffset(); // Read and store virtual ground voltage for current measurement

    uint8_t PWM_Value = 0xD7; //soft start 85% duty

    elapsedTime = 0;

    //Move to B to A position
    analogWrite(CH_RIN, PWM_Value);
    analogWrite(CH_FIN, 0x00);
    delay(60);        // wait for capacitor to charge to not trip over current
    PWM_Value = 0xFF; //soft start 85% duty

    while (elapsedTime < maxCloseTime)
    {
        current = getCH_Current();

        if (current >= I225mA)
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            return ERROR_OVER_CURRENT;
        }

        if (current >= I200mA)
        {
            if (PWM_Value > 0)
                PWM_Value--;
            analogWrite(CH_FIN, PWM_Value); // overcurrent reduce pwm
        }

        if (current < I100mA)
        {
            if (PWM_Value < 0xFF)
                PWM_Value++;
            analogWrite(CH_FIN, PWM_Value); // current low Increase voltage
        }

        if (current <= I005mA) // current  below 5mA  continue to next stage
        {
            analogWrite(CH_RIN, 0x00); // turn off
            analogWrite(CH_FIN, 0x00);
            if (elapsedTime < 1800)
            {
                return ERROR_VALVE_JAM;
            }
            CH_Properties.valveOpen = false;
            return ERROR_NONE;
        }
    }

    analogWrite(CH_RIN, 0x00); // turn off
    analogWrite(CH_FIN, 0x00);

    return ERROR_VALVE_JAM;
}