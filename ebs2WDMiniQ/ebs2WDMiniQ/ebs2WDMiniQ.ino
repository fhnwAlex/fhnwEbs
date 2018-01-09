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

	////FOR TIMING TESTS
	//Serial.print("CC");
	//Serial.print("\t");
	//Serial.print("Min.UI");
	//Serial.print("\t");
	//Serial.print("Max.UI");
	//Serial.print("\t");
	//Serial.print("Min.Col");
	//Serial.print("\t");
	//Serial.print("Max.Col");
	//Serial.print("\t");
	//Serial.print("Min.Ang");
	//Serial.print("\t");
	//Serial.print("Max.Ang");
	//Serial.print("\t");
	//Serial.print("Min.Mo");
	//Serial.print("\t");
	//Serial.print("Max.Mo");
	//Serial.print("\t");
	//Serial.print("Min.Li");
	//Serial.print("\t");
	//Serial.print("Max.Li");
	//Serial.print("\t");
	//Serial.print("Min.LCD");
	//Serial.print("\t");
	//Serial.print("Max.LCD");
	//Serial.print("\t");
	//Serial.print("Min.LCD_UD");
	//Serial.print("\t");
	//Serial.println("Max.LCD_UD");
}

////FOR TIMING TESTS
//unsigned long ulCtfgetAngle = 0;
//unsigned long ulCtOldfgetAngle = 0;
//unsigned long ulCtfgetAngleMin = 10000000;
//unsigned long ulCtfgetAngleMax = 0;
//
//unsigned long ulCtfUIProcedure = 0;
//unsigned long ulCtOldfUIProcedure = 0;
//unsigned long ulCtfUIProcedureMin = 10000000;
//unsigned long ulCtfUIProcedureMax = 0;
//
//unsigned long ulCtfsetColor = 0;
//unsigned long ulCtOldfsetColor = 0;
//unsigned long ulCtfsetColorMin = 10000000;
//unsigned long ulCtfsetColorMax = 0;
//
//unsigned long ulCtfMoveProcedure = 0;
//unsigned long ulCtOldfMoveProcedure = 0;
//unsigned long ulCtfMoveProcedureMin = 10000000;
//unsigned long ulCtfMoveProcedureMax = 0;
//
//unsigned long ulCtfgetLight = 0;
//unsigned long ulCtOldfgetLight = 0;
//unsigned long ulCtfgetLightMin = 10000000;
//unsigned long ulCtfgetLightMax = 0;
//
//unsigned long ulCtfUpdateDisplay = 0;
//unsigned long ulCtOldfUpdateDisplay = 0;
//unsigned long ulCtfUpdateDisplayMin = 10000000;
//unsigned long ulCtfUpdateDisplayMax = 0;
//
//unsigned long ulTimeUpdateLcdMin = 10000000;
//unsigned long ulTimeUpdateLcdMax = 0;
//
//unsigned long ulCycleCount = 1;



// the loop function runs over and over again until power down or reset
void loop() 
{
	////FOR TIMING TESTS
	///*CYCLIC FUNCTIONS*/
	//ulCtOldfUIProcedure = micros();
	//fUIProcedure(&stPrivate);
	//ulCtfUIProcedure = micros();

	//ulCtOldfsetColor = micros();
	//fsetColor(&stPrivate);
	//ulCtfsetColor = micros();

	//if (pstUI->bStartAuto || pstUI->bStartManual)
	//{

	//	ulCtOldfgetAngle = micros();
	//	fgetAngle(&stPrivate.stCompass);
	//	ulCtfgetAngle = micros();
	//	
	//	ulCtOldfMoveProcedure = micros();
	//	fMoveProcedure(&stPrivate.stMotor);
	//	ulCtfMoveProcedure = micros();

	//	ulCtOldfgetLight = micros();
	//	fgetLight(&stPrivate.stLight);
	//	ulCtfgetLight = micros();

	//	ulCtOldfUpdateDisplay = micros();
	//	fUpdateDisplay(&stPrivate);
	//	ulCtfUpdateDisplay = micros();
	//}

	//ulCtfUIProcedure = ulCtfUIProcedure - ulCtOldfUIProcedure;
	//ulCtfsetColor = ulCtfsetColor - ulCtOldfsetColor;
	//ulCtfgetAngle = ulCtfgetAngle - ulCtOldfgetAngle;
	//ulCtfMoveProcedure = ulCtfMoveProcedure - ulCtOldfMoveProcedure;
	//ulCtfgetLight = ulCtfgetLight - ulCtOldfgetLight;
	//ulCtfUpdateDisplay = ulCtfUpdateDisplay - ulCtOldfUpdateDisplay;

	//if (ulCtfUIProcedure < ulCtfUIProcedureMin) ulCtfUIProcedureMin = ulCtfUIProcedure;
	//if (ulCtfUIProcedure > ulCtfUIProcedureMax) ulCtfUIProcedureMax = ulCtfUIProcedure;

	//if (ulCtfsetColor < ulCtfsetColorMin) ulCtfsetColorMin = ulCtfsetColor;
	//if (ulCtfsetColor > ulCtfsetColorMax) ulCtfsetColorMax = ulCtfsetColor;

	//if (ulCtfgetAngle < ulCtfgetAngleMin) ulCtfgetAngleMin = ulCtfgetAngle;
	//if (ulCtfgetAngle > ulCtfgetAngleMax) ulCtfgetAngleMax = ulCtfgetAngle;

	//if (ulCtfMoveProcedure < ulCtfMoveProcedureMin) ulCtfMoveProcedureMin = ulCtfMoveProcedure;
	//if (ulCtfMoveProcedure > ulCtfMoveProcedureMax) ulCtfMoveProcedureMax = ulCtfMoveProcedure;

	//if (ulCtfgetLight < ulCtfgetLightMin) ulCtfgetLightMin = ulCtfgetLight;
	//if (ulCtfgetLight > ulCtfgetLightMax) ulCtfgetLightMax = ulCtfgetLight;

	//if (ulCtfUpdateDisplay < ulCtfUpdateDisplayMin) ulCtfUpdateDisplayMin = ulCtfUpdateDisplay;
	//if (ulCtfUpdateDisplay > ulCtfUpdateDisplayMax) ulCtfUpdateDisplayMax = ulCtfUpdateDisplay;

	//if (stPrivate.stUI.ulTimeDiff < ulTimeUpdateLcdMin) ulTimeUpdateLcdMin = stPrivate.stUI.ulTimeDiff;
	//if (stPrivate.stUI.ulTimeDiff > ulTimeUpdateLcdMax) ulTimeUpdateLcdMax = stPrivate.stUI.ulTimeDiff;

	//if (ulCycleCount == 1000)
	//{
	//	Serial.print(ulCycleCount);
	//	Serial.print("\t");
	//	Serial.print(ulCtfUIProcedureMin);
	//	Serial.print("\t");
	//	Serial.print(ulCtfUIProcedureMax);
	//	Serial.print("\t");
	//	Serial.print(ulCtfsetColorMin);
	//	Serial.print("\t");
	//	Serial.print(ulCtfsetColorMax);
	//	Serial.print("\t");
	//	Serial.print(ulCtfgetAngleMin);
	//	Serial.print("\t");
	//	Serial.print(ulCtfgetAngleMax);
	//	Serial.print("\t");
	//	Serial.print(ulCtfMoveProcedureMin);
	//	Serial.print("\t");
	//	Serial.print(ulCtfMoveProcedureMax);
	//	Serial.print("\t");
	//	Serial.print(ulCtfgetLightMin);
	//	Serial.print("\t");
	//	Serial.print(ulCtfgetLightMax);
	//	Serial.print("\t");
	//	Serial.print(ulCtfUpdateDisplayMin);
	//	Serial.print("\t");
	//	Serial.print(ulCtfUpdateDisplayMax);
	//	Serial.print("\t");
	//	Serial.print(ulTimeUpdateLcdMin);
	//	Serial.print("\t");
	//	Serial.println(ulTimeUpdateLcdMax);
	//
	//	ulCtfgetAngleMin = 10000000;
	//	ulCtfgetAngleMax = 0;
	//	ulCtfUIProcedureMin = 10000000;
	//	ulCtfUIProcedureMax = 0;
	//	ulCtfsetColorMin = 10000000;
	//	ulCtfsetColorMax = 0;
	//	ulCtfMoveProcedureMin = 10000000;
	//	ulCtfMoveProcedureMax = 0;
	//	ulCtfgetLightMin = 10000000;
	//	ulCtfgetLightMax = 0;
	//	ulCtfUpdateDisplayMin = 10000000;
	//	ulCtfUpdateDisplayMax = 0;
	//	ulTimeUpdateLcdMin = 10000000;
	//	ulTimeUpdateLcdMax = 0;

	//}

	//ulCycleCount++;

	//if (ulCycleCount > 1000) ulCycleCount = 1;


	//UNCOMMENT AFTER TIMING TESTS
	/*CYCLIC FUNCTIONS*/
	fUIProcedure(&stPrivate);
    fsetUIMenu(&stPrivate);
	fsetColor(&stPrivate);
    fDisplayProcedure(pstUI);
    fgetAngle(&stPrivate.stCompass);
    fMoveProcedure(&stPrivate.stMotor);
    fgetLight(&stPrivate.stLight);

    //Serial.print("UI State: ");
    //Serial.print("\t");
    //Serial.println(pstUI->enUIState);

	//if (pstUI->bStartAuto)
	//{
	//	fgetAngle(&stPrivate.stCompass);
	//	fMoveProcedure(&stPrivate.stMotor);
	//	fgetLight(&stPrivate.stLight);

 //   }
 //   else if (pstUI->bStartManual)
 //   {
 //       fgetAngle(&stPrivate.stCompass);
 //       fMoveProcedure(&stPrivate.stMotor);
 //       fgetLight(&stPrivate.stLight);

 //   }
}
