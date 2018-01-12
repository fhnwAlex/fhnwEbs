/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program:    Functions.cpp               */
/*  Revision:   1.0                         */
/*  Author:     A. Gessler / B. Hürzeler    */
/*  Date:       2018-01-12                  */
/**********************************************************************************************************************/
/* Includes
**********************************************************************************************************************/
#include <math.h>
#include "string.h"
#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include "Wire.h"
#include "I2Cdev.h"
#include <HMC5883L.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

/**********************************************************************************************************************/
/* Defines
**********************************************************************************************************************/
#define FORW            0           // Motor control forward
#define BACK            1           // Motor control backward
#define LCDADDR         0x20        // Adress for LCD display
#define LCDROWS         2           // Number of rows on LCD
#define LCDCOLS         16          // Number of coloums on LCD
#define SPEEDPINLEFT    5           // Pin for run the left motor 
#define SPEEDPINRIGHT   6           // Pin for run the right motor 
#define DIRECTIONRIGHT  7           // Pin for control right motor direction
#define DIRECTIONLEFT   12          // Pin for control left motor direction
#define MIN_V           30.0        // Minimum speed of motors
#define MAX_V           60.0        // Maximum speed of motors
#define ANGLE_MIN       10.0        // Minimum angle
#define ANGLE_MAX       350.0       // Maximum angle
#define HALFCIRCLE      180.0       // Half circle in degrees
#define LEDPIN          10          // Pin of RGB-LED
#define CALIB_TIME      20000       // Calibration time [ms]
#define DEGREES         223         // ASCII Number for '°' char

/**********************************************************************************************************************/
/* Instances
**********************************************************************************************************************/
HMC5883L mag;
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS);
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);

/**********************************************************************************************************************/
/* Functions
**********************************************************************************************************************/
void fWriteSingleValue(tstPrvMain *pstPrivate, char szStringLine[], unsigned int uiValue, unsigned char uchLcdRow);
void fWriteDoubleValue(tstPrvMain *pstPrivate, char szStringLine[], unsigned int uiValue_1, float flValue_2, unsigned char uchLcdRow);
void fWriteString(tstPrvMain *pstPrivate, char szStringLine[], unsigned char uchLcdRow);
/**********************************************************************************************************************/

void finitUp(tstPrvMain *pstPrivate)
/****************************************************************
Initialize after Power up

*****************************************************************/
{
    // Pointer access to (interfaces)
    pstPrivate->stMotor.puiActAngle = &pstPrivate->stCompass.uiAngle;
    pstPrivate->stUI.puiActAngle = &pstPrivate->stCompass.uiAngle;
    pstPrivate->stRgbLed.puiColor = &pstPrivate->stCompass.uiAngle;
    pstPrivate->stUI.pflLightInVoltage = &pstPrivate->stLight.flLightInVoltage;

    // Initialize LCD-Display
    uint8_t uiLcdSquare[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
    lcd.begin();
    lcd.backlight();
    lcd.home();
    lcd.createChar(0, uiLcdSquare);

    // Initialize compass
    mag.initialize();

    // Initialize Motorpins
    pinMode(SPEEDPINLEFT, OUTPUT);
    pinMode(SPEEDPINRIGHT, OUTPUT);
    pinMode(DIRECTIONRIGHT, OUTPUT);
    pinMode(DIRECTIONLEFT, OUTPUT);

    // Initialize RGB-LED
    led.begin();
    led.show();
    led.setBrightness(60);

    // Fill up Buffer with zeros
    memset(&pstPrivate->stUI.szDisplayData[0], '0', sizeof(pstPrivate->stUI.szDisplayData));

    // Set UI-State to Start - Up
    pstPrivate->stUI.enUIState = enUIState_StartUp;
};

void fMoveProcedure(tstMotor *pstMotor)
/****************************************************************
Move procedure of the motors with arguments of direction and speed
Static / Dynamic speed
*****************************************************************/
{
    // Static speed mode for calibration
    if (!pstMotor->bCompassCalibrated)
    {
        if (pstMotor->bCalibRun)
        {
            digitalWrite(DIRECTIONRIGHT, FORW);
            analogWrite(SPEEDPINRIGHT, MIN_V);
            delayMicroseconds(10);
            digitalWrite(DIRECTIONLEFT, BACK);
            analogWrite(SPEEDPINLEFT, MIN_V);
        }
        else if (pstMotor->bCalibRunL)
        {
            digitalWrite(DIRECTIONRIGHT, BACK);
            analogWrite(SPEEDPINRIGHT, MIN_V);
            delayMicroseconds(10);
            digitalWrite(DIRECTIONLEFT, FORW);
            analogWrite(SPEEDPINLEFT, MIN_V);
        }
        else
        {
            analogWrite(SPEEDPINRIGHT, LOW);
            analogWrite(SPEEDPINLEFT, LOW);
        }
    }
    else if (pstMotor->bRun)
    {
        // Dynamic speed mode
        signed int	iDiffAngle = *pstMotor->puiActAngle;
        iDiffAngle = abs(iDiffAngle - 180);

        // Calculate actual speed
        pstMotor->uiSpeed = (unsigned int)(((MAX_V - MIN_V) / (-HALFCIRCLE) * (float)iDiffAngle) + MAX_V);
        if (iDiffAngle >= (HALFCIRCLE - ANGLE_MIN))
        {
            analogWrite(SPEEDPINRIGHT, LOW);
            analogWrite(SPEEDPINLEFT, LOW);
        }
        else if ((*pstMotor->puiActAngle < ANGLE_MAX) && (*pstMotor->puiActAngle > HALFCIRCLE))
        {
            // Left turn
            digitalWrite(DIRECTIONRIGHT, BACK);
            analogWrite(SPEEDPINRIGHT, pstMotor->uiSpeed);
            delayMicroseconds(10);
            digitalWrite(DIRECTIONLEFT, FORW);
            analogWrite(SPEEDPINLEFT, pstMotor->uiSpeed);
        }
        else
        {
            // Right turn
            digitalWrite(DIRECTIONRIGHT, FORW);
            analogWrite(SPEEDPINRIGHT, pstMotor->uiSpeed);
            delayMicroseconds(10);
            digitalWrite(DIRECTIONLEFT, BACK);
            analogWrite(SPEEDPINLEFT, pstMotor->uiSpeed);
        }
    }
    else
    {
        analogWrite(SPEEDPINRIGHT, LOW);
        analogWrite(SPEEDPINLEFT, LOW);
    }
};

void fsetColor(tstPrvMain *pstPrivate)
/****************************************************************
Indicates color on RGB Led
Areas and colors:Red = Far away / Yellow = Near North / Green = North
*****************************************************************/
{
    tstRgbLed *pstColor = &pstPrivate->stRgbLed;
    tstUI *pstUI = &pstPrivate->stUI;
    tstMotor *pstMotor = &pstPrivate->stMotor;

    signed int	iColor = *pstColor->puiColor;
    iColor = abs(iColor - 180);

    if (pstUI->bStartAuto || pstUI->bStartManual)
    {
        if (iColor <= 90) { led.setPixelColor(0, 200, 0, 0); }
        else if ((iColor > 90) && (iColor < 170)) { led.setPixelColor(0, 255, 204, 0); }
        else if (iColor >= 170) { led.setPixelColor(0, 0, 200, 0); }
        led.show();
    }
    else if (pstMotor->bCalibRun || pstMotor->bCalibRunL)
    {
        if ((pstColor->ulCycle % 200) < 100)
        {
            led.setPixelColor(0, 255, 255, 255);
            led.show();
        }
        else if ((pstColor->ulCycle % 100) < 100)
        {
            led.setPixelColor(0, 0, 0, 0);
            led.show();
        }
        pstColor->ulCycle++;
    }
    else led.setPixelColor(0, 0, 0, 0);
    led.show();
};

void fcompassCalibrate(tstPrvMain *pstPrivate)
/****************************************************************
Compass calibration / Offset calculating
Run for every new location after Power ON
*****************************************************************/
{
    tstCompass *pstCompass = &pstPrivate->stCompass;
    tstMotor *pstMotor = &pstPrivate->stMotor;

    bool            bStarted = false;
    signed int      iMaxX, iMaxY = 0;
    signed int      iMinX, iMinY = 1000;
    unsigned long   ulStartTime = millis();

    pstMotor->bCalibRun = true;
    fMoveProcedure(pstMotor);       // Start turn robot to the right

    for (unsigned int i = 0; (millis() - ulStartTime) < CALIB_TIME;)
    {
        if ((millis() - ulStartTime) > (CALIB_TIME / 2) && !bStarted)
        {
            bStarted = true;
            pstMotor->bCalibRunL = true;
            pstMotor->bCalibRun = false;
            fMoveProcedure(pstMotor);       // Start turn robot to the left
        }
        mag.getHeading(&pstCompass->iMagnet_x, &pstCompass->iMagnet_y, &pstCompass->iMagnet_z);
        if (pstCompass->iMagnet_x < iMinX) { iMinX = pstCompass->iMagnet_x; }
        if (pstCompass->iMagnet_x > iMaxX) { iMaxX = pstCompass->iMagnet_x; }
        if (pstCompass->iMagnet_y < iMinY) { iMinY = pstCompass->iMagnet_y; }
        if (pstCompass->iMagnet_y > iMaxY) { iMaxY = pstCompass->iMagnet_y; }

        // Visualize calibration progress on LCD-Display
        if (i < LCDCOLS && (millis() - ulStartTime) >= (CALIB_TIME / LCDCOLS * i))
        {
            lcd.setCursor(i, 1);
            lcd.write(0);
            i++;
        }
        fsetColor(pstPrivate);      // LED blink indication during calibration
    }

    // Calculate offsets
    pstCompass->iMagOffset_x = (iMaxX + iMinX) / 2;
    pstCompass->iMagOffset_y = (iMaxY + iMinY) / 2;

    pstMotor->bCalibRun = false;
    pstMotor->bCalibRunL = false;
    pstCompass->bCalibDone = true;
    pstMotor->bCompassCalibrated = pstCompass->bCalibDone;
    fMoveProcedure(pstMotor);       // Stop movement after calibration
    lcd.setCursor(0, 0);
};

void fgetAngle(tstCompass *pstCompass)
/****************************************************************
Get actual angle from compass

*****************************************************************/
{
    float flTempAngle = 0.0;

    // Set declination angle on your location and fix heading
    // You can find your declination on: http://magnetic-declination.com/
    // (+) Positive or (-) for negative
    // Formula: (deg + (min / 60.0)) / (180 / M_PI);
    pstCompass->flDeclinationAngle = (2.0 + (16.0 / 60.0)) / (180.0 / M_PI);

    // Read raw heading measurements from device
    mag.getHeading(&pstCompass->iMagnet_x, &pstCompass->iMagnet_y, &pstCompass->iMagnet_z);
    pstCompass->iMagnet_x = pstCompass->iMagnet_x - pstCompass->iMagOffset_x;
    pstCompass->iMagnet_y = pstCompass->iMagnet_y - pstCompass->iMagOffset_y;

    // To calculate heading in degrees. 0 degree indicates North
    flTempAngle += (atan2((float)pstCompass->iMagnet_y, (float)pstCompass->iMagnet_x) +
                   (pstCompass->flDeclinationAngle) +
                   (90.0 * M_PI / 180.0));

    if (flTempAngle < 0) { flTempAngle += 2 * M_PI; }

    // Calculation from radiant to degree
    flTempAngle = flTempAngle * 180 / M_PI;

    // Write TempAngle into struct variable
    pstCompass->uiAngle = (unsigned int)flTempAngle;
};

void fgetLight(tstLightSensor *pstLight)
/****************************************************************
Get the light value and convert it into voltage for UI
Visualize the voltage over the Photoresistance
*****************************************************************/
{
    pstLight->flLightInVoltage = ((float)analogRead(5) / 1024.0) * 5.0;
};

unsigned short fgetKeyValue(tstUI *pstUIKey)
/****************************************************************
Determined which Key was pressed / Set key state

*****************************************************************/
{
    unsigned short  usRet = (analogRead(6) * 5) / 1023;
    static int keys[4] = { 0,0,0,0 };

    if (usRet > 4)
    {
        pstUIKey->enKeyState = enKey_undef;
        if (++keys[0] >= 1)
        {
            if (keys[1] > 700) { usRet = 4; }
            else if (keys[1] > 10) { usRet = 1; }
            else if (keys[2] > 10) { usRet = 2; }
            else if (keys[3] > 10) { usRet = 3; }
            else usRet = 0;
            if (usRet) { keys[0] = keys[1] = keys[2] = keys[3] = 0; }
        }
        else usRet = pstUIKey->enKeyState;
    }
    else
    {
        if (usRet >= 0 && usRet < 1)
        {
            if (keys[1]<32000) ++keys[1];
            pstUIKey->enKeyState = enKey_1;
            usRet = pstUIKey->enKeyState;
        }
        else if (usRet >= 1 && usRet < 3)
        {
            ++keys[2];
            pstUIKey->enKeyState = enKey_2;
            usRet = pstUIKey->enKeyState;
        }
        else if (usRet >= 3 && usRet < 4)
        {
            ++keys[3];
            pstUIKey->enKeyState = enKey_3;
            usRet = pstUIKey->enKeyState;
        }
        usRet = pstUIKey->enKeyState;
    }
    return usRet;
};

void fUIProcedure(tstPrvMain *pstPrivate)
/****************************************************************
Complete User Interface procedure
Set UI-States and handle the output interface of the UI module
*****************************************************************/
{
    tstCompass *pstCompass = &pstPrivate->stCompass;
    tstMotor *pstMotor = &pstPrivate->stMotor;
    tstUI *pstUIProcedure = &pstPrivate->stUI;

    // Read key state from function
    pstUIProcedure->usKeyState = fgetKeyValue(pstUIProcedure);

    switch (pstUIProcedure->usKeyState)
    {
    case enKey_1:

        if (!pstCompass->bCalibDone)
        {
            pstUIProcedure->enUIState = enUIState_Calibration;
            pstUIProcedure->uchPrevState = pstUIProcedure->enUIState;
        }
        else if (!pstUIProcedure->bStartAuto || !pstUIProcedure->bStartManual)
        {
            pstUIProcedure->bStartManual = true;
            pstUIProcedure->enUIState = enUIState_ManualMode;
            pstUIProcedure->uchPrevState = pstUIProcedure->enUIState;
        }
        break;

    case enKey_2:
        if (pstCompass->bCalibDone && !pstUIProcedure->bStartManual)
        {
            pstUIProcedure->bStartAuto = true;
            pstMotor->bRun = true;
            pstUIProcedure->enUIState = enUIState_AutomaticMode;
            pstUIProcedure->uchPrevState = pstUIProcedure->enUIState;
        }
        break;

    case enKey_3:
        if (pstCompass->bCalibDone && (pstUIProcedure->uchPrevState != enUIState_Abort))
        {
            pstUIProcedure->ulTimeTextOnHold = millis();
            pstUIProcedure->bStartAuto = false;
            pstUIProcedure->bStartManual = false;
            pstUIProcedure->bModeAborted = true;
            pstMotor->bRun = false;
            fMoveProcedure(pstMotor);
            pstUIProcedure->enUIState = enUIState_Abort;
            pstUIProcedure->uchPrevState = pstUIProcedure->enUIState;
        }
        break;

    case enKey_undef:
        break;

    default:
        break;
    }
};

void fsetUIMenu(tstPrvMain *pstPrivate)
/****************************************************************
Indicate different User Interface menus / Depend on UI-State
Write String lines into DataBuffer (Cyclic)
*****************************************************************/
{
    tstCompass	*pstCompass = &pstPrivate->stCompass;
    tstUI       *pstUI = &pstPrivate->stUI;

    switch (pstUI->enUIState)
    {
    case enUIState_StartUp:
        fWriteString(pstPrivate, "PRESS Key 1 to  ", 1);
        fWriteString(pstPrivate, "calib. compass! ", 2);
        break;

    case enUIState_Wait:
        fWriteString(pstPrivate, "Key1 - ManMode  ", 1);
        fWriteString(pstPrivate, "Key2 - AutoMode ", 2);
        break;

    case enUIState_Calibration:
        lcd.clear();
        lcd.print("Compass calibr. ");
        fcompassCalibrate(pstPrivate);
        lcd.setCursor(0, 0);
        lcd.print("   Complete!    ");
        delay(2000);
        pstUI->enUIState = enUIState_Wait;
        break;

    case enUIState_ManualMode:
        fWriteDoubleValue(pstPrivate, "Ang:XXX  L:X.XX ", *pstPrivate->stMotor.puiActAngle, *pstUI->pflLightInVoltage, 1);
        fWriteString(pstPrivate, "KEY3 - STOP     ", 2);
        break;

    case enUIState_AutomaticMode:
        fWriteSingleValue(pstPrivate, "Act. Angle: XXX ", *pstPrivate->stMotor.puiActAngle, 1);
        fWriteString(pstPrivate, "KEY3 - STOP     ", 2);
        break;

    case enUIState_Abort:
        if ((millis() - pstUI->ulTimeTextOnHold) < 2000)
        {
            fWriteString(pstPrivate, "MODE ABORTED!   ", 1);
            fWriteString(pstPrivate, "                ", 2);
        }
        else
        {
            pstUI->bModeAborted = false;
            pstUI->enUIState = enUIState_Wait;
        }
        break;

    default:
        break;
    }
};

void fDisplayProcedure(tstUI *pstDisplay)
/****************************************************************
Display text driver / Handle the LCD functions /
sending each char per cycle
*****************************************************************/
{
    if (pstDisplay->ulCycle == sizeof(pstDisplay->szDisplayData))
    {
        //Set cursor to 1st line
        lcd.setCursor(0, 0);
        pstDisplay->bSetCursorFlag = true;
    }

    if (pstDisplay->ulCycle == LCDCOLS)
    {
        // Set cursor to 2nd line
        lcd.setCursor(0, 1);
        pstDisplay->bSetCursorFlag = true;
    }

    if (pstDisplay->ulCycle > sizeof(pstDisplay->szDisplayData))
    {
        // Reset index
        pstDisplay->uchDisplayIx = 0;
        pstDisplay->ulCycle = 0;
    }

    if (!pstDisplay->bSetCursorFlag)
    {
        // Send each char to LCD-Display
        lcd.print(pstDisplay->szDisplayData[pstDisplay->uchDisplayIx]);
        pstDisplay->uchDisplayIx++;
    }

    pstDisplay->bSetCursorFlag = false;
    pstDisplay->ulCycle++;
};

void fWriteSingleValue(tstPrvMain *pstPrivate, char szStringLine[], unsigned int uiValue, unsigned char uchLcdRow)
/****************************************************************
Function for create and write a string which has one value /
Converting value into chars and write into DataBuffer
*****************************************************************/
{
    tstUI *pstSingleValue = &pstPrivate->stUI;

    memcpy(&pstSingleValue->szDisplayData[(uchLcdRow - 1) * LCDCOLS], &szStringLine[0], strlen(szStringLine));
    pstSingleValue->szDisplayData[uchLcdRow * LCDCOLS] = '\0';

    // Convert integer (3 digit) to chars
    pstSingleValue->uchFirstDigit_SV = uiValue / 100;
    pstSingleValue->uchSecondDigit_SV = (uiValue - (pstSingleValue->uchFirstDigit_SV * 100)) / 10;
    pstSingleValue->uchThirdDigit_SV = (uiValue - (pstSingleValue->uchFirstDigit_SV * 100) - (pstSingleValue->uchSecondDigit_SV * 10) / 1);

    // Insert Angle chars into string (ASCII 48 == '0')
    if (pstSingleValue->uchFirstDigit_SV == 0)
    {
        if (pstSingleValue->uchSecondDigit_SV == 0)
        {
            pstSingleValue->szDisplayData[12] = ' ';
            pstSingleValue->szDisplayData[13] = ' ';
            pstSingleValue->szDisplayData[14] = ('0' + pstSingleValue->uchThirdDigit_SV);
        }
        else
        {
            pstSingleValue->szDisplayData[12] = ' ';
            pstSingleValue->szDisplayData[13] = ('0' + pstSingleValue->uchSecondDigit_SV);
            pstSingleValue->szDisplayData[14] = ('0' + pstSingleValue->uchThirdDigit_SV);
        }
    }
    else 
    {
        pstSingleValue->szDisplayData[12] = ('0' + pstSingleValue->uchFirstDigit_SV);
        pstSingleValue->szDisplayData[13] = ('0' + pstSingleValue->uchSecondDigit_SV);
        pstSingleValue->szDisplayData[14] = ('0' + pstSingleValue->uchThirdDigit_SV);
    }
    pstSingleValue->szDisplayData[15] = DEGREES;    // Degrees char (°)
}

void fWriteDoubleValue(tstPrvMain *pstPrivate, char szStringLine[], unsigned int uiValue_1, float flValue_2, unsigned char uchLcdRow)
/****************************************************************
Function for create and write a string which has two values
Converting values into chars and write into DataBuffer
*****************************************************************/
{
    tstUI *pstDoubleValue = &pstPrivate->stUI;

    memcpy(&pstDoubleValue->szDisplayData[(uchLcdRow - 1) * LCDCOLS], &szStringLine[0], strlen(szStringLine));
    pstDoubleValue->szDisplayData[uchLcdRow * LCDCOLS] = '\0';

    // Convert Angle integer (3 digit) to chars
    pstDoubleValue->uchAngleFirstDigit_DV =   uiValue_1 / 100;
    pstDoubleValue->uchAngleSecondDigit_DV = (uiValue_1 - (pstDoubleValue->uchAngleFirstDigit_DV * 100)) / 10;
    pstDoubleValue->uchAngleThirdDigit_DV =  (uiValue_1 - (pstDoubleValue->uchAngleFirstDigit_DV * 100) -
                                             (pstDoubleValue->uchAngleSecondDigit_DV * 10) / 1);

    // Convert Voltage integer (3 digit) to chars
    pstDoubleValue->uchVoltageFirstDigit_DV =    flValue_2;
    pstDoubleValue->uchVoltageSecondDigit_DV = ((flValue_2 * 100) - (pstDoubleValue->uchVoltageFirstDigit_DV * 100)) / 10;;
    pstDoubleValue->uchVoltageThirdDigit_DV =  ((flValue_2 * 100) - (pstDoubleValue->uchVoltageFirstDigit_DV * 100) -
                                               ( pstDoubleValue->uchVoltageSecondDigit_DV * 10) / 1);
    
    // Insert Angle chars into string (ASCII 48 == '0')
    if (pstDoubleValue->uchAngleFirstDigit_DV == 0)
    {
        if (pstDoubleValue->uchAngleSecondDigit_DV == 0)
        {
            pstDoubleValue->szDisplayData[4] = ' ';
            pstDoubleValue->szDisplayData[5] = ' ';
            pstDoubleValue->szDisplayData[6] = ('0' + pstDoubleValue->uchAngleThirdDigit_DV);
        }
        else
        {
            pstDoubleValue->szDisplayData[4] = ' ';
            pstDoubleValue->szDisplayData[5] = ('0' + pstDoubleValue->uchAngleSecondDigit_DV);
            pstDoubleValue->szDisplayData[6] = ('0' + pstDoubleValue->uchAngleThirdDigit_DV);
        }
    }
    else
    {
        pstDoubleValue->szDisplayData[4] = ('0' + pstDoubleValue->uchAngleFirstDigit_DV);
        pstDoubleValue->szDisplayData[5] = ('0' + pstDoubleValue->uchAngleSecondDigit_DV);
        pstDoubleValue->szDisplayData[6] = ('0' + pstDoubleValue->uchAngleThirdDigit_DV);
    }
    pstDoubleValue->szDisplayData[7] = DEGREES;    // Degrees char

    // Insert Voltage chars into string (ASCII 48 == '0')
    pstDoubleValue->szDisplayData[11] = ('0' + pstDoubleValue->uchVoltageFirstDigit_DV);
    pstDoubleValue->szDisplayData[12] = '.';
    pstDoubleValue->szDisplayData[13] = ('0' + pstDoubleValue->uchVoltageSecondDigit_DV);
    pstDoubleValue->szDisplayData[14] = ('0' + pstDoubleValue->uchVoltageThirdDigit_DV);
    pstDoubleValue->szDisplayData[15] = 'V';
};

void fWriteString(tstPrvMain *pstPrivate, char szStringLine[], unsigned char uchLcdRow)
/****************************************************************
Function for create and write a string which has no values

*****************************************************************/
{
    tstUI *pstString = &pstPrivate->stUI;

    memcpy(&pstString->szDisplayData[(uchLcdRow - 1) * LCDCOLS], &szStringLine[0], strlen(szStringLine));
    pstString->szDisplayData[uchLcdRow * LCDCOLS] = '\0';
};
