/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program :: Functions.cpp	*/
/*	Revision:: 1.0	*/
/*	Author:: A. Gessler / B. Hürzeler	*/
/*	Date:: 2017-10-05	*/
/**********************************************************************************************************************/
/* Includes
**********************************************************************************************************************/
//#include <functions.h>
#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include "Wire.h"
#include "I2Cdev.h"
#include <HMC5883L.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

/**********************************************************************************************************************/
/* Defines
**********************************************************************************************************************/
#define FORW			0			// Motor control forward
#define BACK			1			// Motor control backward
#define LCDROWS			2			// Number of rows on LCD
#define SpeedPinLeft	5			// Pin for run the left motor 
#define SpeedPinRight	6			// Pin for run the right motor 
#define DirectionRight	7			// Pin for control right motor direction
#define LEDPIN			10			// Pin of RGB-LED
#define DirectionLeft	12			// Pin for control left motor direction
#define LCDCOLS			16			// Number of coloums on LCD
#define TONEPIN			16			// Pin of buzzer
#define SPEEDMAX		255			// Maximum speed of motors
#define LCDADDR			0x20		// Adress for LCD display

/**********************************************************************************************************************/
/* Instances
**********************************************************************************************************************/
HMC5883L mag;
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS);
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);


/*****************************************************************/
void finitUp(void)
/****************************************************************
Initialize after Power up

*****************************************************************/
{
	tstPrvMain *pstPrivate, stPrivate;
	pstPrivate = &stPrivate;
	
	memset(&stPrivate, 0, sizeof(stPrivate));		// Set whole privat memory 0

	lcd.begin();
	lcd.backlight();
	lcd.home();

};

/*****************************************************************/
int fcompassCalibrate(void)
/****************************************************************
Compass calibration
Run for every new location
*****************************************************************/
{
	
};

/*****************************************************************/
void fMoveProcedure(int direction, unsigned int speed)
/****************************************************************
Move procedure of a motor with arguments of direction and speed

*****************************************************************/
{
	tstMotor *pstMotor, stMotor;
	pstMotor = &stMotor;

	pstMotor->uiDirection = 0;
	pstMotor->uiSpeed = 0;

};

void fsetTone(void)
/****************************************************************
Generate and set a tone on the buzzer

*****************************************************************/
{
	tstBuzzer *pstBuzzer, stBuzzer;
	pstBuzzer = &stBuzzer;


};

/*****************************************************************/
unsigned short fgetKeyValue(void)
/****************************************************************
Determined which Key was pressed

*****************************************************************/
{
	tstUI	*pstUI, stUI;
	pstUI = &stUI;

	unsigned short		usRet = (analogRead(6) * 5) / 1023;
	static int keys[4] = { 0,0,0,0 };

	if (usRet > 4)
	{
		lcd.setCursor(12, 0);
		lcd.print("key0");//tests only
		if (++keys[0] >= 1)
		{
			if (keys[1] > 700) usRet = 4;
			else if (keys[1] > 10) usRet = 1;
			else if (keys[2] > 10) usRet = 2;
			else if (keys[3] > 10) usRet = 3;
			else usRet = 0;
			if (usRet) keys[0] = keys[1] = keys[2] = keys[3] = 0;
		}
		else usRet = 0;
	}
	else
	{
		if (usRet >= 0 && usRet < 1)
		{
			if (keys[1]<32000) ++keys[1];
			lcd.setCursor(12, 0);
			lcd.print("key1");	//tests only
		}
		else if (usRet >= 1 && usRet < 3)
		{
			++keys[2];
			lcd.setCursor(12, 0);
			lcd.print("key2");//tests only
		}
		else if (usRet >= 3 && usRet < 4)
		{
			++keys[3];
			lcd.setCursor(12, 0);
			lcd.print("key3");//tests only
		}
		usRet = 0;
	}
	return usRet;
	lcd.print(usRet);
};



