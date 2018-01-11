/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS 
**********************************************************************************************************************
/*  Program:    ebs2WDMiniQ.ino             */
/*  Revision:   1.0                         */
/*  Author:     A. Gessler / B. Hürzeler    */
/*  Date:       2018-01-12                  */
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

/****************************************************************
MEMORY ALLOCATION
*****************************************************************/
tstPrvMain stPrivate;	//Allocate private memory
tstUI *pstUI = &stPrivate.stUI;

// the setup function runs once when you press reset or power the board
void setup() 
{
    // Set whole privat memory 0
    memset(&stPrivate, 0, sizeof(stPrivate));
    // Call init function
    finitUp(&stPrivate);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
    /*CYCLIC FUNCTIONS*/
    fUIProcedure(&stPrivate);
    fgetLight(&stPrivate.stLight);
    fgetAngle(&stPrivate.stCompass);
    fMoveProcedure(&stPrivate.stMotor);
    fsetUIMenu(&stPrivate);
    fDisplayProcedure(pstUI);
    fsetColor(&stPrivate);
}
