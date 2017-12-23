/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program :: Functions.h	*/
/*	Revision:: 1.0	*/
/*	Author:: A. Gessler / B. Hürzeler	*/
/*	Date:: 2017-10-05	*/
/**********************************************************************************************************************/
/* Implementierung des Controllers
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
	enKey_undef = 0,
	enKey_1,
	enKey_2,
	enKey_3,
}tenKey;

/*----------------------------------------------------------------
User Interface enumerator
----------------------------------------------------------------*/
typedef enum tenUIState
{
	enUIState_undef = 0,
	enUIState_Calibration,
	enUIState_ManualMode,
	enUIState_AutomaticMode,
	enUIState_Abort,
}tenUIState;

/*----------------------------------------------------------------
Motor control structure
----------------------------------------------------------------*/
typedef struct tstMotor
{
	unsigned int 	uiSpeed;        //
	unsigned int	uiAngelNorth;	//
	unsigned int	*puiActAngle;
	bool			bRun;
	bool			bCalibRun;
	bool			bCompassCalibrated;
}tstMotor;

/*----------------------------------------------------------------
Buzzer structure
----------------------------------------------------------------*/
typedef struct tstBuzzer
{
	unsigned long		ulToneDurration;
	unsigned long		ulToneFrequency;
}tstBuzzer;

/*----------------------------------------------------------------
RGB-LED structure
----------------------------------------------------------------*/
typedef struct tstRgbLed
{
	

}tstRgbLed;

/*----------------------------------------------------------------
Compass structure
----------------------------------------------------------------*/
typedef struct tstCompass
{
	unsigned int	uiAngle;
	unsigned int	uiSamples;
	unsigned int	uiMagOffset_x;
	unsigned int	uiMagOffset_y;
	float			flDeclinationAngle;
	signed int		iMagnet_x;
	signed int		iMagnet_y;
	signed int		iMagnet_z;
	bool			bRun;
	bool			bCalibRun;
	bool			bCalibDone;
}tstCompass;

/*----------------------------------------------------------------
User interface structure
----------------------------------------------------------------*/
typedef struct tstUI
{
	tenKey			enKeyState;
	tenUIState		enUIState;
	tstBuzzer		stBuzzer;
	tstMotor		stMotor;
	tstCompass		stCompass;
	unsigned int	*puiActAngle;
	unsigned short	usPrevState;
	bool			bInitUpDone;
	bool			bMenuSet; //for test only!!
	bool			bStartAuto;
	bool			bStartManual;
	bool			bStartCalibrate;
	bool			bRun;
	bool			bCalibRun;


}tstUI;


/*----------------------------------------------------------------
Private structure
----------------------------------------------------------------*/
typedef struct tstPrvMain
{
	tstMotor		stMotor;
	tstBuzzer		stBuzzer;
	tstRgbLed		stRgbLed;
	tstUI			stUI;
	tstCompass		stCompass;
}tstPrvMain;

#endif // !FUNCTIONS_H
