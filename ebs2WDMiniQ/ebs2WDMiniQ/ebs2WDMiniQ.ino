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

//###################################################################
//testfunctions
//###################################################################
void fRandomAngleTest(tstUI *pstRandomAngle);


/****************************************************************
MEMORY ALLOCATION
*****************************************************************/
tstPrvMain stPrivate;	//Allocate private memory

tstPrvMain *pstPrivate = &stPrivate;
tstUI *pstUI = &stPrivate.stUI;
tstUI *pstRandomAngle = &stPrivate.stUI; //for tests only!!
tstBuzzer *pstBuzzer = &stPrivate.stBuzzer;
tstMotor *pstMotor = &stPrivate.stMotor;
tstCompass *pstCompass = &stPrivate.stCompass;

// the setup function runs once when you press reset or power the board
void setup() 
{
	memset(&stPrivate, 0, sizeof(stPrivate));		// Set whole privat memory 0
	finitUp(pstPrivate);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	fUIProcedure(pstPrivate);
	fgetAngle(pstCompass);
	fMoveProcedure(pstMotor);
	
	if (pstUI->bStartAuto || pstUI->bStartManual)	fRandomAngleTest(pstRandomAngle);
	
}
