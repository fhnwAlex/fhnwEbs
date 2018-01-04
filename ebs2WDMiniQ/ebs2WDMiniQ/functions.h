/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program:    Functions.h                 */
/*	Revision:   1.0                         */
/*	Author:     A. Gessler / B. Hürzeler    */
/*	Date:       2018-01-12                  */
/**********************************************************************************************************************/
/* IMPLEMENTATION OF CONTROLLER / STRUCTURE
**********************************************************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/*----------------------------------------------------------------
Key enumerator
----------------------------------------------------------------*/
typedef enum tenKey
{
    enKey_undef = 0,                            // State - No key pressed
    enKey_1,                                    // State - Key 1 pressed
    enKey_2,                                    // State - Key 2 pressed
    enKey_3,                                    // State - Key 3 pressed
}tenKey;

/*----------------------------------------------------------------
User Interface enumerator
----------------------------------------------------------------*/
typedef enum tenUIState
{
    enUIState_undef = 0,                        // State - Undefined
    enUIState_Calibration,                      // State - Calibration
    enUIState_ManualMode,                       // State - Manual mode
    enUIState_AutomaticMode,                    // State - Automatic mode
    enUIState_Abort,                            // State - Aborting
}tenUIState;

/*----------------------------------------------------------------
Motor control structure
----------------------------------------------------------------*/
typedef struct tstMotor
{
    bool                bRun;                   // Start motors dynamic speed
    bool                bCalibRun;              // Start motors static right turn
    bool                bCalibRunL;             // Start motors static left turn
    bool                bCompassCalibrated;     // Compass calibration successfull
    unsigned int        uiSpeed;                // Actual speed of motors
    unsigned int       *puiActAngle;            // Actual angle of compass
}tstMotor;

/*----------------------------------------------------------------
Lightsensor structure
----------------------------------------------------------------*/
typedef struct tstLightSensor
{
    unsigned int        uiLightInVoltage;        // Voltage depend on Photodiode resistance
}tstLightSensor;

/*----------------------------------------------------------------
RGB-LED structure
----------------------------------------------------------------*/
typedef struct tstRgbLed
{
    unsigned int       *puiColor;               // Actual color of LED
    unsigned long       ulCycle;                // Cycle counter 	
}tstRgbLed;

/*----------------------------------------------------------------
Compass structure
----------------------------------------------------------------*/
typedef struct tstCompass
{
    bool                bCalibDone;             // Calibration of compass done
    float               flDeclinationAngle;     // Factor for offset calculation
    signed int          iMagnet_x;              // Compass magnitude X-Axis
    signed int          iMagnet_y;              // Compass magnitude Y-Axis
    signed int          iMagnet_z;              // Compass magnitude Z-Axis
    signed int          iMagOffset_x;           // Offset magnitude X-Axis
    signed int          iMagOffset_y;           // Offset magnitude Y-Axis
    unsigned int        uiAngle;                // Actual angle of compass
}tstCompass;

/*----------------------------------------------------------------
User interface structure
----------------------------------------------------------------*/
typedef struct tstUI
{
    bool                bMenuSet;               // For UI-Menu controll
    bool                bStartAuto;             // Start automatic mode
    bool                bStartManual;           // Start manual mode
    bool                bUIDone;                // LCD write done
    tenKey              enKeyState;             // Key states
    tenUIState          enUIState;              // User Interface states
    unsigned int       *puiLightInVoltage;      // Voltage depend on Photodiode resistance
    unsigned int       *puiActAngle;            // Actual angle compass
    unsigned short      usPrevState;            // Previous UI state
    unsigned short      usKeyState;             // Key state
    unsigned long       ulCycle;                // Cycle counter

    char                szAngle[4];
    char                szVoltage[5];
    char                szDisplayData[33];
    char                szOldDisplayData[33];
    unsigned char       uchDisplayIx;
    unsigned char       uchFirstDigit_SV;
    unsigned char       uchSecondDigit_SV;
    unsigned char       uchThirdDigit_SV;
    unsigned char       uchFirstDigit_DV;
    unsigned char       uchSecondDigit_DV;
    unsigned char       uchThirdDigit_DV;

    unsigned long       ulTime;                 //DELETE AFTER TESTING!!
    unsigned long       ulOldTime;              //DELETE AFTER TESTING!!
    unsigned long       ulTimeDiff;             //DELETE AFTER TESTING!!
}tstUI;


/*----------------------------------------------------------------
Private structure
----------------------------------------------------------------*/
typedef struct tstPrvMain
{
    tstMotor            stMotor;                // Motor structure
    tstLightSensor      stLight;                // Lightsensor structure
    tstRgbLed           stRgbLed;               // LED structure
    tstUI               stUI;                   // User Interface structure
    tstCompass          stCompass;              // Compass structure
}tstPrvMain;

#endif // !FUNCTIONS_H
