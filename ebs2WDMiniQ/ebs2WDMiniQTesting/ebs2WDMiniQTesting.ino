/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program :: ebs2WDMiniQ.ino	*/
/*	Revision:: 1.0	*/
/*	Author:: A. Gessler / B. H�rzeler	*/
/*	Date:: 2017-10-05	*/
/**********************************************************************************************************************/
/* Main program
***********************************************************************************************************************/

/****************************************************************
INCLUDES
*****************************************************************/
#include "functions_test.h"

/****************************************************************
FUNCTIONS
*****************************************************************/
void finitUp(tstPrvMain *pstPrivate);
void fcompassCalibrate(tstPrvMain *pstCalibrate);
void fgetAngle(tstCompass *pstCompass);
void fMoveProcedure(tstMotor *pstMotor);
void fsetTone(tstPrvMain *pstBuzzer);
void fsetColor(tstPrvMain *pstLed);
unsigned short fgetKeyValue(tstUI *pstUI);
void fUIProcedure(tstPrvMain *pstUI);
void fUpdateDisplay(tstPrvMain *pstDisplay);


/****************************************************************
MEMORY ALLOCATION
*****************************************************************/
tstPrvMain stPrivate;	//Allocate private memory

tstUI *pstUI = &stPrivate.stUI;

// the setup function runs once when you press reset or power the board
void setup()
{
	memset(&stPrivate, 0, sizeof(stPrivate));		// Set whole privat memory 0
	finitUp(&stPrivate);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	unsigned int uiStart_t = millis();
	/*CYCLIC FUNCTIONS*/
	//Serial.print("Serialprint: ");
	//Serial.print((uiStart_t - millis()));

	fUIProcedure(&stPrivate);
	//Serial.print("  fUIPr.: ");
	//Serial.print((uiStart_t - millis()));

	//fgetAngle(&stPrivate.stCompass);
	//fMoveProcedure(&stPrivate.stMotor);

	fsetColor(&stPrivate);
	//Serial.print("Time: ");
	//Serial.println((uiStart_t - millis()));

	if (pstUI->bStartAuto || pstUI->bStartManual)
	{
		fgetAngle(&stPrivate.stCompass);
		//Serial.print("  fgetAngle: ");
		//Serial.print((uiStart_t - millis()));
		fMoveProcedure(&stPrivate.stMotor);
		//Serial.print("  fMoveProcedure: ");
		//Serial.print((uiStart_t - millis()));
		fUpdateDisplay(&stPrivate);
		//Serial.print("  fUpdateDisplay: ");
		//Serial.println((uiStart_t - millis()));
	}


}
