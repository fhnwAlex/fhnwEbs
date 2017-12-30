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
void fcompassCalibrate(tstPrvMain *pstPrivate);
void fgetAngle(tstCompass *pstCompass);
void fMoveProcedure(tstMotor *pstMotor);
void fsetTone(tstPrvMain *pstBuzzer);
void fsetColor(tstPrvMain *pstPrivate);
unsigned short fgetKeyValue(tstUI *pstUI);
void fUIProcedure(tstPrvMain *pstPrivate);
void fUpdateDisplay(tstPrvMain *pstPrivate);


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


unsigned long ulCycleTime = 0;
unsigned long ulTimeOld = micros();
unsigned long ulCycleTimeSum = 0;
unsigned long ulCycleTimeMin = 10000000;
unsigned long ulCycleTimeMax = 0;
unsigned long ulCycleCount = 1;



// the loop function runs over and over again until power down or reset
void loop() 
{
	//unsigned int uiStart_t = millis();
	///*CYCLIC FUNCTIONS*/
	////Serial.print("Serialprint: ");
	////Serial.print((uiStart_t - millis()));

	//fUIProcedure(&stPrivate);
	////Serial.print("  fUIPr.: ");
	////Serial.print((uiStart_t - millis()));

	////fgetAngle(&stPrivate.stCompass);
	////fMoveProcedure(&stPrivate.stMotor);

	//fsetColor(&stPrivate);
	////Serial.print("Time: ");
	////Serial.println((uiStart_t - millis()));

	//if (pstUI->bStartAuto || pstUI->bStartManual)
	//{
	//	fgetAngle(&stPrivate.stCompass);
	//	//Serial.print("  fgetAngle: ");
	//	//Serial.print((uiStart_t - millis()));
	//	fMoveProcedure(&stPrivate.stMotor);
	//	//Serial.print("  fMoveProcedure: ");
	//	//Serial.print((uiStart_t - millis()));
	//	fUpdateDisplay(&stPrivate);
	//	//Serial.print("  fUpdateDisplay: ");
	//	//Serial.println((uiStart_t - millis()));
	//}


	/*CYCLIC FUNCTIONS*/
	fUIProcedure(&stPrivate);
	//fgetAngle(&stPrivate.stCompass);
	//fMoveProcedure(&stPrivate.stMotor);
	fsetColor(&stPrivate);

	if (pstUI->bStartAuto || pstUI->bStartManual)
	{
		//Time from getAngle to MoveProcedure
		ulTimeOld = micros();

		fgetAngle(&stPrivate.stCompass);

		//Time after getting Angle
		//ulCycleTime = micros() - ulTimeOld;

		fMoveProcedure(&stPrivate.stMotor);

		//Time after MoveProcedure
		ulCycleTime = micros() - ulTimeOld;

		fUpdateDisplay(&stPrivate);
	}

	//Time after everything
	//ulCycleTime = micros() - ulTimeOld;

	/*ulCycleTimeSum += ulCycleTime;
	if (ulCycleTime < ulCycleTimeMin) ulCycleTimeMin = ulCycleTime;
	if (ulCycleTime > ulCycleTimeMax) ulCycleTimeMax = ulCycleTime;
	if (ulCycleCount == 1000)
	{
		ulCycleTimeSum = (ulCycleTimeSum / ulCycleCount);
		Serial.print("Cycle Nr.: ");
		Serial.print(ulCycleCount);
		Serial.print("\t");
		Serial.print("Mean CT: ");
		Serial.print(ulCycleTimeSum);
		Serial.print("\t");
		Serial.print("Min. CT: ");
		Serial.print(ulCycleTimeMin);
		Serial.print("\t");
		Serial.print("Max. CT: ");
		Serial.println(ulCycleTimeMax);

		ulCycleTimeSum = 0;
		ulCycleTimeMin = 10000000;
		ulCycleTimeMax = 0;
		ulCycleTime = 0;
	}
	ulCycleCount++;

	if (ulCycleCount > 1000) ulCycleCount = 1;*/
	//Time for everything and MoveProcedure
	//ulTimeOld = micros();




}
