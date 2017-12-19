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

void finitUp(void);
void fMoveProcedure(int direction, unsigned int speed);
void fsetTone(void);
int fcompassCalibrate(void);
unsigned short fgetKeyValue(void);

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
Motor control structure
----------------------------------------------------------------*/
typedef struct tstMotor
{
	unsigned int 	uiSpeed;        //
	unsigned int	uiDirection;	//
}tstMotor;

/*----------------------------------------------------------------
Buzzer structure
----------------------------------------------------------------*/
typedef struct tstBuzzer
{


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

}tstUI;

/*----------------------------------------------------------------
LCD-Display structure
----------------------------------------------------------------*/
typedef struct tstLcd
{
	unsigned short		uchDummy;

}tstLcd;

/*----------------------------------------------------------------
Compass structure
----------------------------------------------------------------*/
typedef struct tstCompass
{


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
