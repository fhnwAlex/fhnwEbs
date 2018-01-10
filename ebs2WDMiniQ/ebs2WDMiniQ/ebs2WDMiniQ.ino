/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS 
**********************************************************************************************************************
/*  Program:	ebs2WDMiniQ.ino				*/
/*	Revision:	1.0							*/
/*	Author:		A. Gessler / B. Hürzeler	*/
/*	Date:		2018-01-12					*/
/**********************************************************************************************************************/
/* MAIN PROGRAM
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
void fgetLight(tstLightSensor *pstLight);
void fsetColor(tstPrvMain *pstPrivate);
unsigned short fgetKeyValue(tstUI *pstUI);
void fUIProcedure(tstPrvMain *pstPrivate);
void fsetUIMenu(tstPrvMain *pstPrivate);
void fDisplayProcedure(tstUI *pstDisplay);
//void fUpdateDisplay(tstPrvMain *pstPrivate);

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

	//FOR TIMING TESTS
	Serial.print("CC");
	Serial.print("\t");
	//Serial.print("Min.UI");
	//Serial.print("\t");
	//Serial.print("Max.UI");
	//Serial.print("\t");
	//Serial.print("Min.Li");
	//Serial.print("\t");
	//Serial.print("Max.Li");
	//Serial.print("\t");
	//Serial.print("Min.Ang");
	//Serial.print("\t");
	//Serial.print("Max.Ang");
	//Serial.print("\t");
	//Serial.print("Min.Mot");
	//Serial.print("\t");
	//Serial.print("Max.Mot");
	//Serial.print("\t");
	Serial.print("Min.Buff");
	Serial.print("\t");
	Serial.println("Max.Buff");
	//Serial.print("\t");
	//Serial.print("Min.LCD");
	//Serial.print("\t");
	//Serial.print("Max.LCD");
	//Serial.print("\t");
	//Serial.print("Min.LED");
	//Serial.print("\t");
	//Serial.print("Max.LED");

}

//FOR TIMING TESTS

//unsigned long ulCtfUIProcedure = 0;
//unsigned long ulCtOldfUIProcedure = 0;
//unsigned long ulCtfUIProcedureMin = 10000000;
//unsigned long ulCtfUIProcedureMax = 0;

//unsigned long ulCtfgetLight = 0;
//unsigned long ulCtOldfgetLight = 0;
//unsigned long ulCtfgetLightMin = 10000000;
//unsigned long ulCtfgetLightMax = 0;

//unsigned long ulCtfgetAngle = 0;
//unsigned long ulCtOldfgetAngle = 0;
//unsigned long ulCtfgetAngleMin = 10000000;
//unsigned long ulCtfgetAngleMax = 0;

//unsigned long ulCtfMoveProcedure = 0;
//unsigned long ulCtOldfMoveProcedure = 0;
//unsigned long ulCtfMoveProcedureMin = 10000000;
//unsigned long ulCtfMoveProcedureMax = 0;

//unsigned long ulCtfsetUIMenu = 0;
//unsigned long ulCtOldfsetUIMenu = 0;
//unsigned long ulCtfsetUIMenuMin = 10000000;
//unsigned long ulCtfsetUIMenuMax = 0;

unsigned long ulCtfDisplayPro = 0;
unsigned long ulCtOldfDisplayPro = 0;
unsigned long ulCtfDisplayProMin = 10000000;
unsigned long ulCtfDisplayProMax = 0;

//unsigned long ulCtfsetColor = 0;
//unsigned long ulCtOldfsetColor = 0;
//unsigned long ulCtfsetColorMin = 10000000;
//unsigned long ulCtfsetColorMax = 0;

unsigned long ulCycleCount = 1;



// the loop function runs over and over again until power down or reset
void loop() 
{
	////FOR TIMING TESTS
	///*CYCLIC FUNCTIONS*/

	//ulCtOldfUIProcedure = micros();
	fUIProcedure(&stPrivate);
	//ulCtfUIProcedure = micros();

	//ulCtOldfgetLight = micros();
	fgetLight(&stPrivate.stLight);
	//ulCtfgetLight = micros();

	//ulCtOldfgetAngle = micros();
	fgetAngle(&stPrivate.stCompass);
	//ulCtfgetAngle = micros();

	//ulCtOldfMoveProcedure = micros();
	fMoveProcedure(&stPrivate.stMotor);
	//ulCtfMoveProcedure = micros();

	//ulCtOldfsetUIMenu = micros();
	fsetUIMenu(&stPrivate);
	//ulCtfsetUIMenu = micros();

	ulCtOldfDisplayPro = micros();
	fDisplayProcedure(pstUI);
	ulCtfDisplayPro = micros();

	//ulCtOldfsetColor = micros();
	fsetColor(&stPrivate);
	//ulCtfsetColor = micros();


	//ulCtfUIProcedure = ulCtfUIProcedure - ulCtOldfUIProcedure;
	//ulCtfgetLight = ulCtfgetLight - ulCtOldfgetLight;
	//ulCtfgetAngle = ulCtfgetAngle - ulCtOldfgetAngle;
	//ulCtfMoveProcedure = ulCtfMoveProcedure - ulCtOldfMoveProcedure;
	//ulCtfsetUIMenu = ulCtfsetUIMenu - ulCtOldfsetUIMenu;
	ulCtfDisplayPro = ulCtfDisplayPro - ulCtOldfDisplayPro;
	//ulCtfsetColor = ulCtfsetColor - ulCtOldfsetColor;

	//if (ulCtfUIProcedure < ulCtfUIProcedureMin) ulCtfUIProcedureMin = ulCtfUIProcedure;
	//if (ulCtfUIProcedure > ulCtfUIProcedureMax) ulCtfUIProcedureMax = ulCtfUIProcedure;

	//if (ulCtfgetLight < ulCtfgetLightMin) ulCtfgetLightMin = ulCtfgetLight;
	//if (ulCtfgetLight > ulCtfgetLightMax) ulCtfgetLightMax = ulCtfgetLight;

	//if (ulCtfgetAngle < ulCtfgetAngleMin) ulCtfgetAngleMin = ulCtfgetAngle;
	//if (ulCtfgetAngle > ulCtfgetAngleMax) ulCtfgetAngleMax = ulCtfgetAngle;

	//if (ulCtfMoveProcedure < ulCtfMoveProcedureMin) ulCtfMoveProcedureMin = ulCtfMoveProcedure;
	//if (ulCtfMoveProcedure > ulCtfMoveProcedureMax) ulCtfMoveProcedureMax = ulCtfMoveProcedure;

	//if (ulCtfsetUIMenu < ulCtfsetUIMenuMin) ulCtfsetUIMenuMin = ulCtfsetUIMenu;
	//if (ulCtfsetUIMenu > ulCtfsetUIMenuMax) ulCtfsetUIMenuMax = ulCtfsetUIMenu;

	if (ulCtfDisplayPro < ulCtfDisplayProMin) ulCtfDisplayProMin = ulCtfDisplayPro;
	if (ulCtfDisplayPro > ulCtfDisplayProMax) ulCtfDisplayProMax = ulCtfDisplayPro;

	//if (ulCtfsetColor < ulCtfsetColorMin) ulCtfsetColorMin = ulCtfsetColor;
	//if (ulCtfsetColor > ulCtfsetColorMax) ulCtfsetColorMax = ulCtfsetColor;

	if (ulCycleCount == 1000)
	{
		Serial.print(ulCycleCount);
		Serial.print("\t");
		//Serial.print(ulCtfUIProcedureMin);
		//Serial.print("\t");
		//Serial.print(ulCtfUIProcedureMax);
		//Serial.print("\t");
		//Serial.print(ulCtfgetLightMin);
		//Serial.print("\t");
		//Serial.print(ulCtfgetLightMax);
		//Serial.print("\t");
		//Serial.print(ulCtfgetAngleMin);
		//Serial.print("\t");
		//Serial.print(ulCtfgetAngleMax);
		//Serial.print("\t");
		//Serial.print(ulCtfMoveProcedureMin);
		//Serial.print("\t");
		//Serial.print(ulCtfMoveProcedureMax);
		//Serial.print("\t");
		//Serial.print(ulCtfsetUIMenuMin);
		//Serial.print("\t");
		//Serial.print(ulCtfsetUIMenuMax);
		//Serial.print("\t");
		Serial.print(ulCtfDisplayProMin);
		Serial.print("\t");
		Serial.println(ulCtfDisplayProMax);
		//Serial.print("\t");
		//Serial.print(ulCtfsetColorMin);
		//Serial.print("\t");
		//Serial.println(ulCtfsetColorMax);
	

		//ulCtfUIProcedureMin = 10000000;
		//ulCtfUIProcedureMax = 0;
		//ulCtfgetLightMin = 10000000;
		//ulCtfgetLightMax = 0;
		//ulCtfgetAngleMin = 10000000;
		//ulCtfgetAngleMax = 0;
		//ulCtfMoveProcedureMin = 10000000;
		//ulCtfMoveProcedureMax = 0;
		//ulCtfsetUIMenuMin = 10000000;
		//ulCtfsetUIMenuMax = 0;
		ulCtfDisplayProMin = 10000000;
		ulCtfDisplayProMax = 0;
		//ulCtfsetColorMin = 10000000;
		//ulCtfsetColorMax = 0;

	}

	ulCycleCount++;

	if (ulCycleCount > 1000) ulCycleCount = 1;


	////UNCOMMENT AFTER TIMING TESTS
	///*CYCLIC FUNCTIONS*/
	//fUIProcedure(&stPrivate);
	//fsetUIMenu(&stPrivate);
	//fsetColor(&stPrivate);
	//fDisplayProcedure(pstUI);
	//fgetAngle(&stPrivate.stCompass);
	//fMoveProcedure(&stPrivate.stMotor);
	//fgetLight(&stPrivate.stLight);

    //Serial.print("UI State: ");
    //Serial.print("\t");
    //Serial.println(pstUI->enUIState);
}
