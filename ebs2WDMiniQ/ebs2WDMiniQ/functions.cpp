/**********************************************************************************************************************
FHNW - EMBEDDED SYSTEMS
**********************************************************************************************************************
/*  Program :: Functions.cpp	*/
/*	Revision:: 1.0	*/
/*	Author:: A. Gessler / B. H�rzeler	*/
/*	Date:: 2017-10-05	*/
/**********************************************************************************************************************/
/* Includes
**********************************************************************************************************************/
#include <math.h>
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
#define TONEFREQ		200			// Tone frequency buzzer
#define LCDADDR			0x20		// Adress for LCD display

/**********************************************************************************************************************/
/* Instances
**********************************************************************************************************************/
HMC5883L mag;
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS);
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);

/*****************************************************************/
void finitUp(tstPrvMain *pstPrivate)
/****************************************************************
Initialize after Power up

*****************************************************************/
{
	pstPrivate->stMotor.pflActAngle = &pstPrivate->stCompass.flAngle;
	pstPrivate->stUI.pflActAngle = &pstPrivate->stCompass.flAngle;
	
	// Initialize LCD-Display
	uint8_t				uiLcdSquare[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
	lcd.begin();
	lcd.backlight();
	lcd.home();

	lcd.createChar(0, uiLcdSquare);

	// Initialize compass
	mag.initialize();
	if (!mag.testConnection())	lcd.println("Connection to compass failed!");

	// Initialize Motors
	pinMode(SpeedPinLeft, OUTPUT);
	pinMode(SpeedPinRight, OUTPUT);
	pinMode(DirectionRight, OUTPUT);
	pinMode(DirectionLeft, OUTPUT);

	// Initialize RGB-LED
	led.begin();
	led.show();

	// Initialize Buzzer
	pinMode(TONEPIN, OUTPUT);

};

/*****************************************************************/
void fcompassCalibrate(tstCompass *pstCompass)
/****************************************************************
Compass calibration
Run for every new location
*****************************************************************/
{
	
};

void fgetAngle(tstCompass *pstCompass)
/****************************************************************
Get actual angle from compass

*****************************************************************/
{
	pstCompass->flAngle = 0.0;
	pstCompass->uiSamples = 10;

	// Set declination angle on your location and fix heading
	// You can find your declination on: http://magnetic-declination.com/
	// (+) Positive or (-) for negative
	// Formula: (deg + (min / 60.0)) / (180 / M_PI);
	pstCompass->declinationAngle = (2.0 + (16.0 / 60.0)) / (180.0 / M_PI);

	for (int i = 1; i <= pstCompass->uiSamples; i++) {
		// read raw heading measurements from device
		mag.getHeading(&pstCompass->iMagnet_x, &pstCompass->iMagnet_y, &pstCompass->iMagnet_z);

		//mx = mx - (-326); //Offset Brugg Projektraum
		//my = my - 288; //Offset Brugg Proejktraum
		pstCompass->iMagnet_x = pstCompass->iMagnet_x - 291; //Offset Birmi
		pstCompass->iMagnet_y = pstCompass->iMagnet_y - 321; //Offset Birmi

	   // To calculate heading in degrees. 0 degree indicates North
		pstCompass->flAngle += (atan2(pstCompass->iMagnet_y, pstCompass->iMagnet_x) + pstCompass->declinationAngle);
	}
	pstCompass->flAngle = pstCompass->flAngle / pstCompass->uiSamples;

	if (pstCompass->flAngle < 0) { pstCompass->flAngle += 2 * M_PI; }
	pstCompass->flAngle = round(pstCompass->flAngle * 180 / M_PI);
};

/*****************************************************************/
void fMoveProcedure(tstMotor *pstMotor)
/****************************************************************
Move procedure of a motor with arguments of direction and speed

*****************************************************************/
{
	


};

void fsetTone(tstBuzzer *pstBuzzer)
/****************************************************************
Generate and set a tone on the buzzer

*****************************************************************/
{
	//pstBuzzer->ulToneDurration = ;
	tone(TONEPIN, TONEFREQ, pstBuzzer->ulToneDurration);


};

/*****************************************************************/
unsigned short fgetKeyValue(tstUI *pstUI)
/****************************************************************
Determined which Key was pressed
Set key state
*****************************************************************/
{
	unsigned short		usRet = (analogRead(6) * 5) / 1023;
	static int keys[4] = { 0,0,0,0 };

	if (usRet > 4)
	{
		pstUI->enKeyState = enKey_undef;
		if (++keys[0] >= 1)
		{
			if (keys[1] > 700) usRet = 4;
			else if (keys[1] > 10) usRet = 1;
			else if (keys[2] > 10) usRet = 2;
			else if (keys[3] > 10) usRet = 3;
			else usRet = 0;
			if (usRet) keys[0] = keys[1] = keys[2] = keys[3] = 0;
		}
		else usRet = pstUI->enKeyState;
	}
	else
	{
		if (usRet >= 0 && usRet < 1)
		{
			if (keys[1]<32000) ++keys[1];
			pstUI->enKeyState = enKey_1;
			usRet = pstUI->enKeyState;
		}
		else if (usRet >= 1 && usRet < 3)
		{
			++keys[2];
			pstUI->enKeyState = enKey_2;
			usRet = pstUI->enKeyState;
		}
		else if (usRet >= 3 && usRet < 4)
		{
			++keys[3];
			pstUI->enKeyState = enKey_3;
			usRet = pstUI->enKeyState;
		}
		usRet = pstUI->enKeyState;
	}
	return usRet;
};

void fUIProcedure(tstUI *pstUI)
/****************************************************************
Complete User Interface procedure

*****************************************************************/
{
	unsigned short		usKeyState;
	usKeyState = fgetKeyValue(pstUI);

	switch (usKeyState)
	{
	case enKey_1:
		pstUI->enUIState = enUIState_Calibration;
		//fcompassCalibrate();
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.println("Compass calibr..");
		for (unsigned int i = 0; i < LCDCOLS; i++)
		{
			lcd.setCursor(i, 1);
			lcd.write(0);
			delay(300);
		}
		lcd.setCursor(0, 0);
		lcd.println("                ");
		lcd.setCursor(4, 0);
		lcd.println("Complete!");
		break;

	case enKey_2:
		pstUI->enUIState = enUIState_ManualMode;
		break;

	case enKey_3:
		if (pstUI->enUIState == enUIState_AutomaticMode)
		{
			pstUI->stMotor.bRun = false;
		}
		else
		{
			pstUI->enUIState = enUIState_AutomaticMode;
			pstUI->stMotor.bRun = true;
		}
		break;

	case enKey_undef:
		pstUI->enUIState = enUIState_undef;
		//lcd.println("1.Calib 2.ManMod 3.AutoMod");
		break;


	default:
		break;
	}

};

