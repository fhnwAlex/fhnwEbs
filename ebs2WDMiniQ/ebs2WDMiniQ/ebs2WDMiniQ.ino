/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS 
**********************************************************************************************************************
/*  Program :: ebs2WDMiniQ.ino	*/
/*	Revision:: 1.0	*/
/*	Author:: A. Gessler / B. Hürzeler	*/
/*	Date:: 2017-10-05	*/
/**********************************************************************************************************************/
/* Main program
***********************************************************************************************************************/

/****************************************************************
INCLUDES
*****************************************************************/
#include "functions.h"

/****************************************************************
FUNCTIONS
*****************************************************************/
void finitUp(tstPrvMain *pstPrivate);
void fcompassCalibrate(tstCompass *pstCompass, tstMotor *pstMotor);
void fgetAngle(tstCompass *pstCompass);
void fMoveProcedure(tstMotor *pstMotor);
void fsetTone(tstUI *pstBuzzer);
unsigned short fgetKeyValue(tstUI *pstUI);
void fUIProcedure(tstPrvMain *pstUI);
void fUpdateDisplay(tstUI *pstDisplay);


/****************************************************************
MEMORY ALLOCATION
*****************************************************************/
tstPrvMain stPrivate;	//Allocate private memory

tstUI *pstUI = &stPrivate.stUI;

// the setup function runs once when you press reset or power the board
void setup() 
{
	memset(&stPrivate, 0, sizeof(stPrivate));		// Set whole privat memory 0
	//finitUp(pstPrivate);
	finitUp(&stPrivate);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	/*CYCLIC FUNCTIONS*/
	fUIProcedure(&stPrivate);
	fgetAngle(&stPrivate.stCompass);
	fMoveProcedure(&stPrivate.stMotor);
	if (pstUI->bStartAuto || pstUI->bStartManual)	fUpdateDisplay(&stPrivate.stUI);
}
