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
void fcompassCalibrate(tstCompass *pstCompass);
void fgetAngle(tstCompass *pstCompass);
void fMoveProcedure(tstMotor *pstMotor);
void fsetTone(tstBuzzer *pstBuzzer);
unsigned short fgetKeyValue(tstUI *pstUI);
void fUIProcedure(tstUI *pstUI);

//###################################################################
//testfunctions
//###################################################################
void fRandomAngleTest(tstUI *pstRandomAngle);


/****************************************************************
MEMORY ALLOCATION
*****************************************************************/
tstPrvMain stPrivate;	//Allocate private memory
tstUI *pstUI = &stPrivate.stUI;
tstUI *pstRandomAngle = &stPrivate.stUI;
tstBuzzer *pstBuzzer = &stPrivate.stBuzzer;

// the setup function runs once when you press reset or power the board
void setup() 
{
	tstPrvMain *pstPrivate = &stPrivate;
	memset(&stPrivate, 0, sizeof(stPrivate));		// Set whole privat memory 0
	finitUp(pstPrivate);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	fUIProcedure(pstUI);

	if (pstUI->bStartAuto || pstUI->bStartManual)
	{
		fRandomAngleTest(pstRandomAngle);
	}
	
	fsetTone(pstBuzzer);


}
