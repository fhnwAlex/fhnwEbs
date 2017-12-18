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
#include <functions.h>
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

/**********************************************************************************************************************/
/* Functions
**********************************************************************************************************************/
void fsetMotorSpeed(void);

int fcompassCalibrate(void);




/*****************************************************************/
void finitUp(void)
/****************************************************************
Initialize after Power up

*****************************************************************/
{
	tstPrvMain *pstPrivate, stPrivate;
	pstPrivate = &stPrivate;

	memset(&stPrivate, 0, sizeof(stPrivate));		// Set whole privat memory 0
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
void fsetMotorSpeed(void)
/****************************************************************
Set motor speed

*****************************************************************/
{

};




