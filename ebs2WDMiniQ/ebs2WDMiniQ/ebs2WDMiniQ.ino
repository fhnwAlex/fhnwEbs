/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS - MAIN PROGRAM
**********************************************************************************************************************
/*  Program:    ebs2WDMiniQ.ino             */
/*  Revision:   1.0                         */
/*  Author:     A. Gessler / B. Hürzeler    */
/*  Date:       2018-01-12                  */
/**********************************************************************************************************************/
/*INCLUDES
/*****************************************************************/
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
/*****************************************************************/
tstPrvMain stPrivate;   //Allocate private memory

void setup() 
{
    memset(&stPrivate, 0, sizeof(stPrivate));   // Set whole privat memory 0
    finitUp(&stPrivate);                        // Call init function
}

void loop() 
{
    fUIProcedure(&stPrivate);
    fgetLight(&stPrivate.stLight);
    fgetAngle(&stPrivate.stCompass);
    fMoveProcedure(&stPrivate.stMotor);
    fsetUIMenu(&stPrivate);
    fDisplayProcedure(&stPrivate.stUI);
    fsetColor(&stPrivate);
}