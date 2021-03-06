/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program :: Functions.h	*/
/*	Revision:: 1.0	*/
/*	Author:: A. Gessler / B. H�rzeler	*/
/*	Date:: 2017-10-05	*/
/**********************************************************************************************************************/
/* Implementierung des Controllers
**********************************************************************************************************************/

#ifndef FUNCTIONS_H



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
	tstLcd		stLcd;
	tstKeys		stKeys;

}tstUI;

/*----------------------------------------------------------------
LCD-Display structure
----------------------------------------------------------------*/
typedef struct tstLcd
{


}tstLcd;

/*----------------------------------------------------------------
Keys structure
----------------------------------------------------------------*/
typedef struct tstKeys
{


}tstKeys;

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
	tstKeys			stKeys;
	tstCompass		stCompass;
}tstPrvMain;

#endif // !FUNCTIONS_H