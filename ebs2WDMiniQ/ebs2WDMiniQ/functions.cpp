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
#define ANGLE_MIN		5			// Minimum angle
#define SpeedPinRight	6			// Pin for run the right motor 
#define DirectionRight	7			// Pin for control right motor direction
#define LEDPIN			10			// Pin of RGB-LED
#define SAMPLES			10			// For filter magnitude
#define DirectionLeft	12			// Pin for control left motor direction
#define LCDCOLS			16			// Number of coloums on LCD
#define TONEPIN			16			// Pin of buzzer
#define MIN_V			40			// Minimum speed of motors
#define MAX_V			100			// Maximum speed of motors
//#define SPEEDMAX		255			// Maximum speed of motors
#define TONEFREQ		100			// Tone frequency buzzer
#define ANGLE_MAX		355			// Maximum angle
#define LCDADDR			0x20		// Adress for LCD display

/**********************************************************************************************************************/
/* Instances
**********************************************************************************************************************/
HMC5883L mag;
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS);
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);

void fsetUIMenu(tstUI *pstUIMenu);
/*****************************************************************/
void finitUp(tstPrvMain *pstPrivate)
/****************************************************************
Initialize after Power up

*****************************************************************/
{
	tstUI *pstInitUI = &pstPrivate->stUI;

	// Pointer access to interface
	pstPrivate->stMotor.puiActAngle = &pstPrivate->stCompass.uiAngle;
	pstPrivate->stUI.puiActAngle = &pstPrivate->stCompass.uiAngle;
	pstPrivate->stUI.pbCompassReady = &pstPrivate->stCompass.bCalibrationDone;

	// Initialize LCD-Display
	uint8_t	uiLcdSquare[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
	uint8_t uiLcdDegrees[] = { 0x0C, 0x12, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00 };
	lcd.begin();
	lcd.backlight();
	lcd.home();
	lcd.createChar(0, uiLcdSquare);
	lcd.createChar(1, uiLcdDegrees);

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

	pstPrivate->stUI.enUIState = enUIState_undef;
	fsetUIMenu(pstInitUI);
	pstPrivate->stUI.bInitUpDone = true;
};

/*****************************************************************/
void fcompassCalibrate(tstCompass *pstCompass)
/****************************************************************
Compass calibration
Run for every new location
*****************************************************************/
{
	/*pstCompass->bRun = true;
	fMoveProcedure();*/





	pstCompass->bCalibrationDone = true;

	
};

void fgetAngle(tstCompass *pstCompass)
/****************************************************************
Get actual angle from compass

*****************************************************************/
{
	pstCompass->uiMagOffset_x = 291; //tests only!!
	pstCompass->uiMagOffset_y = 321; //tests only!!

	// Set declination angle on your location and fix heading
	// You can find your declination on: http://magnetic-declination.com/
	// (+) Positive or (-) for negative
	// Formula: (deg + (min / 60.0)) / (180 / M_PI);
	pstCompass->flDeclinationAngle = (2.0 + (16.0 / 60.0)) / (180.0 / M_PI);

	for (int i = 1; i <= SAMPLES; i++) {
		// read raw heading measurements from device
		mag.getHeading(&pstCompass->iMagnet_x, &pstCompass->iMagnet_y, &pstCompass->iMagnet_z);

		//mx = mx - (-326); //Offset Brugg Projektraum
		//my = my - 288; //Offset Brugg Proejktraum
		pstCompass->iMagnet_x = pstCompass->iMagnet_x - pstCompass->uiMagOffset_x;//291; //Offset Birmi
		pstCompass->iMagnet_y = pstCompass->iMagnet_y - pstCompass->uiMagOffset_y;//321; //Offset Birmi

	   // To calculate heading in degrees. 0 degree indicates North
		pstCompass->uiAngle += (atan2(pstCompass->iMagnet_y, pstCompass->iMagnet_x) + pstCompass->flDeclinationAngle);
	}
	pstCompass->uiAngle = pstCompass->uiAngle / SAMPLES;

	if (pstCompass->uiAngle < 0) { pstCompass->uiAngle += 2 * M_PI; }
	pstCompass->uiAngle = round(pstCompass->uiAngle * 180 / M_PI);
};

/*****************************************************************/
void fMoveProcedure(tstMotor *pstMotor)
/****************************************************************
Move procedure of a motor with arguments of direction and speed

*****************************************************************/
{
	tstMotor *pstMotor = &stPrivate.stMotor;

	// Dynamic speed mode
	signed int	siDiffAngle = *pstMotor->puiActAngle - 180;
	unsigned int uiSpeed = (MAX_V - MIN_V) / 180 * abs(siDiffAngle) + MIN_V;

	if (*pstMotor->puiActAngle <= ANGLE_MIN || *pstMotor->puiActAngle >= ANGLE_MAX)
	{
		analogWrite(SpeedPinRight, LOW);
		analogWrite(SpeedPinLeft, LOW);
	}
	if (*pstMotor->puiActAngle < ANGLE_MAX && *pstMotor->puiActAngle >= 180)
	{
		// Rechtsdrehen
		digitalWrite(DirectionRight, FORW);
		analogWrite(SpeedPinRight, uiSpeed);
		delay(2);
		digitalWrite(DirectionLeft, BACK);
		analogWrite(SpeedPinLeft, uiSpeed);
	}
	else
	{
		// Linksdrehen
		digitalWrite(DirectionRight, BACK);
		analogWrite(SpeedPinRight, uiSpeed);
		delay(2);
		digitalWrite(DirectionLeft, FORW);
		analogWrite(SpeedPinLeft, uiSpeed);
	}


};

void fsetTone(tstUI *pstBuzzer)
/****************************************************************
Generate and set a tone on the buzzer

*****************************************************************/
{
	//unsigned int uiAngleDiff;
	//
	//uiAngleDiff = *pstUI->puiActAngle - 180;
	//tone(TONEPIN, abs(uiAngleDiff), pstUI->stBuzzer.ulToneDurration);


};

/*****************************************************************/
unsigned short fgetKeyValue(tstUI *pstUIKey)
/****************************************************************
Determined which Key was pressed
Set key state
*****************************************************************/
{
	unsigned short		usRet = (analogRead(6) * 5) / 1023;
	static int keys[4] = { 0,0,0,0 };

	if (usRet > 4)
	{
		pstUIKey->enKeyState = enKey_undef;
		if (++keys[0] >= 1)
		{
			if (keys[1] > 700) usRet = 4;
			else if (keys[1] > 10) usRet = 1;
			else if (keys[2] > 10) usRet = 2;
			else if (keys[3] > 10) usRet = 3;
			else usRet = 0;
			if (usRet) keys[0] = keys[1] = keys[2] = keys[3] = 0;
		}
		else usRet = pstUIKey->enKeyState;
	}
	else
	{
		if (usRet >= 0 && usRet < 1)
		{
			if (keys[1]<32000) ++keys[1];
			pstUIKey->enKeyState = enKey_1;
			usRet = pstUIKey->enKeyState;
		}
		else if (usRet >= 1 && usRet < 3)
		{
			++keys[2];
			pstUIKey->enKeyState = enKey_2;
			usRet = pstUIKey->enKeyState;
		}
		else if (usRet >= 3 && usRet < 4)
		{
			++keys[3];
			pstUIKey->enKeyState = enKey_3;
			usRet = pstUIKey->enKeyState;
		}
		usRet = pstUIKey->enKeyState;
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
	tstCompass *pstCompass = &stPrivate.stUI.stCompass;
	switch (usKeyState)
	{
	case enKey_1:

		if (!pstUI->bCompassReady)//for test only!! pointer instead of boolean
		{
			//fcompassCalibrate(pstCompass);
			pstUI->bStartCalibrate = true;
			pstUI->bCompassReady = true; //for test only!! simulation -> after set within function above
			pstUI->enUIState = enUIState_Calibration;
			fsetUIMenu(pstUI);
		}
		else
		{
			pstUI->bStartManual = true;
			pstUI->enUIState = enUIState_ManualMode;
			fsetUIMenu(pstUI);
		}
		break;

	case enKey_2:
		if (pstUI->bCompassReady)
		{
			pstUI->bStartAuto = true;
			pstUI->enUIState = enUIState_AutomaticMode;
			if (pstUI->bStartAuto)
			{
				pstUI->enUIState = enUIState_AutomaticMode;
				fsetUIMenu(pstUI);
				delay(100);
			}
			
		}
		break;

	case enKey_3:
		pstUI->bStartAuto = false;
		pstUI->bStartManual = false;

		if(pstUI->bCompassReady)
		{
			pstUI->enUIState = enUIState_Abort;
			fsetUIMenu(pstUI);
		}
		break;

	case enKey_undef:
		pstUI->enUIState = enUIState_undef;
		
		if (!pstUI->bMenuSet)
		{
			fsetUIMenu(pstUI);
		}
		break;
	default:
		break;
	}
};

void fsetUIMenu(tstUI *pstUIMenu)
/****************************************************************
Indicate different User Interface menus

*****************************************************************/
{
	if (pstUIMenu->enUIState == enUIState_Calibration)
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		if (pstUIMenu->bCompassReady)		lcd.println("Compass calibr..");	//for test only!! boolean change to pointer
		for (unsigned int i = 0; i < LCDCOLS; i++)
		{
			lcd.setCursor(i, 1);
			lcd.write(0);
			delay(300);
		}
		lcd.setCursor(0, 0);
		lcd.print("                  ");
		lcd.setCursor(4, 0);
		lcd.print("Complete!");
		delay(1000);
		pstUIMenu->bMenuSet = false;
		pstUIMenu->bStartCalibrate = false;
	}
	else if (pstUIMenu->enUIState == enUIState_undef && !pstUIMenu->bCompassReady)	//for test only!!
	{
		lcd.clear();
		lcd.println("PRESS Key 1 to  ");
		lcd.setCursor(0, 1);
		lcd.println("calib. compass! ");
		pstUIMenu->bMenuSet = true;
	}
	else if(pstUIMenu->enUIState == enUIState_undef && pstUIMenu->bCompassReady)//for test only!!
	{
		lcd.clear();
		lcd.println("Key 1 - ManMode ");
		lcd.setCursor(0, 1);
		lcd.println("Key 2 - AutoMode");
		pstUIMenu->usPrevState = enUIState_undef;
		pstUIMenu->bMenuSet = true;
	}
	else if (pstUIMenu->enUIState == enUIState_ManualMode)	//for test only!!
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("M:");
		pstUIMenu->usPrevState = enUIState_ManualMode;
	}
	else if (pstUIMenu->enUIState == enUIState_AutomaticMode)//for test only!!
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("A:");
		pstUIMenu->usPrevState = enUIState_AutomaticMode;
	}
	else if ((pstUIMenu->enUIState == enUIState_Abort) && pstUIMenu->usPrevState > 0) 
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Mode aborted!");
		delay(2000);
		pstUIMenu->bMenuSet = false;
	}
};

void fRandomAngleTest(tstUI *pstRandomAngle)
//###################################################################
//TESTFUNCTION ->> Simulate an angle (Compass)
//###################################################################
{
	lcd.clear();
	pstRandomAngle->stCompass.uiAngle = random(0, 395);
	lcd.setCursor(0, 0);
	if (pstRandomAngle->bStartManual)	lcd.print("M:Act. Ang:   ");
	if (pstRandomAngle->bStartAuto)		lcd.print("A:Act. Ang:   ");
	lcd.setCursor(12, 0);
	lcd.print(pstRandomAngle->stCompass.uiAngle);
	lcd.setCursor(15, 0);
	lcd.write(1);
	lcd.setCursor(0, 1);
	lcd.print("Key 3 - STOP");
	delay(500);
};