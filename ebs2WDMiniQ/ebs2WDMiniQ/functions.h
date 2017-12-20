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
}tenUIState;

/*----------------------------------------------------------------
Motor control structure
----------------------------------------------------------------*/
typedef struct tstMotor
{
	unsigned int 	uiSpeed;        //
	float			flAngelNorth;	//
	float			*pflActAngle;
	bool			bRun;
}tstMotor;

/*----------------------------------------------------------------
Buzzer structure
----------------------------------------------------------------*/
typedef struct tstBuzzer
{
	unsigned long		ulToneDurration;

}tstBuzzer;

/*----------------------------------------------------------------
RGB-LED structure
----------------------------------------------------------------*/
typedef struct tstRgbLed
{
	

}tstRgbLed;

/*----------------------------------------------------------------
User interface structure
----------------------------------------------------------------*/
typedef struct tstUI
{
	//tstLcd		stLcd;
	tenKey		enKeyState;
	tenUIState	enUIState;
	tstBuzzer	stBuzzer;
	tstMotor	stMotor;
	float		*pflActAngle;

}tstUI;

/*----------------------------------------------------------------
LCD-Display structure
----------------------------------------------------------------*/
typedef struct tstLcd
{
	

}tstLcd;

/*----------------------------------------------------------------
Compass structure
----------------------------------------------------------------*/
typedef struct tstCompass
{
	float			flAngle;
	float			declinationAngle;
	unsigned int	uiSamples;
	signed int		iMagnet_x;
	signed int		iMagnet_y;
	signed int		iMagnet_z;
	bool			bRun;
}tstCompass;

/*----------------------------------------------------------------
Private structure
----------------------------------------------------------------*/
typedef struct tstPrvMain
{
	tstMotor		stMotor;
	tstBuzzer		stBuzzer;
	tstRgbLed		stRgbLed;
	tstUI			stUI;
	tstLcd			stLcd;
	tstCompass		stCompass;
}tstPrvMain;

#endif // !FUNCTIONS_H
