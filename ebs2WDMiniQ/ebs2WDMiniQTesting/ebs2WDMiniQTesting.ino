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

	//FOR TEST Nr. 2
	Serial.print("T LCD Clear");
	Serial.print("\t");
	Serial.print("T LCD Cursor");
	Serial.print("\t");
	Serial.println("T LCD Print");
}


unsigned long ulCycleCount = 1;

// the loop function runs over and over again until power down or reset
void loop()
{
	///*CYCLIC FUNCTIONS*/
	//fUIProcedure(&stPrivate);
	////fgetAngle(&stPrivate.stCompass);
	////fMoveProcedure(&stPrivate.stMotor);
	//fsetColor(&stPrivate);

	//if (pstUI->bStartAuto || pstUI->bStartManual)
	//{
	//	//Time from getAngle to MoveProcedure
	//	ulTimeOld = micros();

	//	fgetAngle(&stPrivate.stCompass);
	//	
	//	//Time after getting Angle
	//	//ulCycleTime = micros() - ulTimeOld;
	//	
	//	fMoveProcedure(&stPrivate.stMotor);

	//	//Time after MoveProcedure
	//	ulCycleTime = micros() - ulTimeOld;
	//	
	//	fUpdateDisplay(&stPrivate);
	//}

	////Time after everything
	////ulCycleTime = micros() - ulTimeOld;

	//ulCycleTimeSum += ulCycleTime;
	//if (ulCycleTime < ulCycleTimeMin) ulCycleTimeMin = ulCycleTime;
	//if (ulCycleTime > ulCycleTimeMax) ulCycleTimeMax = ulCycleTime;
	//if (ulCycleCount == 1000)
	//{
	//	ulCycleTimeSum = (ulCycleTimeSum / ulCycleCount);
	//	Serial.print("Cycle Nr.: ");
	//	Serial.print(ulCycleCount);
	//	Serial.print("\t");
	//	Serial.print("Mean CT: ");
	//	Serial.print(ulCycleTimeSum);
	//	Serial.print("\t");
	//	Serial.print("Min. CT: ");
	//	Serial.print(ulCycleTimeMin);
	//	Serial.print("\t");
	//	Serial.print("Max. CT: ");
	//	Serial.println(ulCycleTimeMax);

	//	ulCycleTimeSum = 0;
	//	ulCycleTimeMin = 10000000;
	//	ulCycleTimeMax = 0;
	//	ulCycleTime = 0;
	//}
	//ulCycleCount++;
	//
	//if(ulCycleCount > 1000) ulCycleCount = 1;

	//TEST NR. 2
	fUpdateDisplay(&stPrivate);
	Serial.print(stPrivate.stUI.ulTimeLCDClearDiff);
	Serial.print("\t");
	Serial.print(stPrivate.stUI.ulTimeLCDCursorDiff);
	Serial.print("\t");
	Serial.println(stPrivate.stUI.ulTimeLCDPrintDiff);
	delay(1000);

}
