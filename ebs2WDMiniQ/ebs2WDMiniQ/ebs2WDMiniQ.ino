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

// the setup function runs once when you press reset or power the board
void setup() 
{
	finitUp();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	fgetKeyValue();
}
