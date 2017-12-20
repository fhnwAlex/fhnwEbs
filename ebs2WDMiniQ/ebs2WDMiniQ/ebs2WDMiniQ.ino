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

#include "functions.h"

tstPrvMain stPrivate;	//Allocate private memory

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
	
	fMoveProcedure();

	fgetAngle();
}
