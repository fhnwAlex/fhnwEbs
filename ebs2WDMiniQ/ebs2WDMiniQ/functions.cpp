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
#define LCDADDR			0x20		// Adress for LCD display
#define LCDROWS			2			// Number of rows on LCD
#define LCDCOLS			16			// Number of coloums on LCD
#define SpeedPinLeft	5			// Pin for run the left motor 
#define SpeedPinRight	6			// Pin for run the right motor 
#define DirectionRight	7			// Pin for control right motor direction
#define DirectionLeft	12			// Pin for control left motor direction
#define MIN_V			30.0		// Minimum speed of motors
#define MAX_V			60.0		// Maximum speed of motors
#define ANGLE_MIN		10.0		// Minimum angle
#define ANGLE_MAX		350			// Maximum angle
#define HALFCIRCLE		180.0		// Half circle in degrees
#define LEDPIN			10			// Pin of RGB-LED
#define SAMPLES			10			// For filter magnitude
#define CALIB_TIME		20000		// Calibration time [ms]
#define TONEPIN			16			// Pin of buzzer
#define TONEFREQ		100			// Tone frequency buzzer

/**********************************************************************************************************************/
/* Instances
**********************************************************************************************************************/
HMC5883L mag;
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS);
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);
/**********************************************************************************************************************/
/* Functions
**********************************************************************************************************************/
void fsetUIMenu(tstPrvMain *pstUIMenu);
/**********************************************************************************************************************/


void finitUp(tstPrvMain *pstPrivate)
/****************************************************************
Initialize after Power up

*****************************************************************/
{
	// Pointer access to interface
	pstPrivate->stMotor.puiActAngle = &pstPrivate->stCompass.uiAngle;
	pstPrivate->stUI.puiActAngle = &pstPrivate->stCompass.uiAngle;
	pstPrivate->stRgbLed.puiColor = &pstPrivate->stCompass.uiAngle;

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

	// Initialize Motorpins
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
	fsetUIMenu(pstPrivate);
	pstPrivate->stUI.bInitUpDone = true;
};

void fMoveProcedure(tstMotor *pstMotor)
/****************************************************************
Move procedure of a motor with arguments of direction and speed

*****************************************************************/
{
	// Static speed mode
	if (!pstMotor->bCompassCalibrated)
	{
		if (pstMotor->bCalibRun)
		{
			digitalWrite(DirectionRight, FORW);
			analogWrite(SpeedPinRight, MIN_V);
			//delay(2);
			delayMicroseconds(500);
			digitalWrite(DirectionLeft, BACK);
			analogWrite(SpeedPinLeft, MIN_V);
		}
		else if (pstMotor->bCalibRunL)
		{
			digitalWrite(DirectionRight, BACK);
			analogWrite(SpeedPinRight, MIN_V);
			//delay(2);
			delayMicroseconds(500);
			digitalWrite(DirectionLeft, FORW);
			analogWrite(SpeedPinLeft, MIN_V);
		}
		else 
		{
			analogWrite(SpeedPinRight, LOW);
			analogWrite(SpeedPinLeft, LOW);
		}
	}else if (pstMotor->bRun)
	{
		// Dynamic speed mode
		signed int	iDiffAngle = *pstMotor->puiActAngle;
		iDiffAngle = abs(iDiffAngle - 180);
		unsigned int uiSpeed = (unsigned int)(((MAX_V - MIN_V) * (float)iDiffAngle / HALFCIRCLE) + MIN_V);

		if (iDiffAngle >= (HALFCIRCLE - ANGLE_MIN))
		{
			analogWrite(SpeedPinRight, LOW);
			analogWrite(SpeedPinLeft, LOW);
		}
		else if ((*pstMotor->puiActAngle < ANGLE_MAX) && (*pstMotor->puiActAngle > HALFCIRCLE)) 
		{
			// Left turn
			digitalWrite(DirectionRight, BACK);
			analogWrite(SpeedPinRight, uiSpeed);
			//delay(2);
			delayMicroseconds(500);
			digitalWrite(DirectionLeft, FORW);
			analogWrite(SpeedPinLeft, uiSpeed);
		}
		else
		{
			// Right turn
			digitalWrite(DirectionRight, FORW);
			analogWrite(SpeedPinRight, uiSpeed);
			//delay(2);
			delayMicroseconds(500);
			digitalWrite(DirectionLeft, BACK);
			analogWrite(SpeedPinLeft, uiSpeed);
		}
	}
	else 
	{
		analogWrite(SpeedPinRight, LOW);
		analogWrite(SpeedPinLeft, LOW);
	}
};

void fsetColor(tstPrvMain *pstPrivate)
/****************************************************************
Indicates color on RGB Led

*****************************************************************/
{
	tstRgbLed *pstColor = &pstPrivate->stRgbLed;
	tstUI *pstUI = &pstPrivate->stUI;
	tstMotor *pstMotor = &pstPrivate->stMotor;

	signed int	iColor = *pstColor->puiColor;
	iColor = abs(iColor - 180);
	led.setBrightness(60);

	if (pstUI->bStartAuto || pstUI->bStartManual)
	{
		if (iColor <= 45)								led.setPixelColor(0, 200, 0, 0);
		else if ((iColor > 45) && (iColor < 170))		led.setPixelColor(0, 255, 204, 0);
		else if (iColor >= 170)							led.setPixelColor(0, 0, 200, 0);
		led.show();
	}
	else if (pstMotor->bCalibRun || pstMotor->bCalibRunL)
	{
		if ((pstColor->ulCycle % 200) < 100)
		{
			led.setPixelColor(0, 255, 0, 255);
			led.show();
		}
		else if ((pstColor->ulCycle % 100) < 100)
		{
			led.setPixelColor(0, 0, 0, 0);
			led.show();
		}
		pstColor->ulCycle++;
	}
	else led.setPixelColor(0, 0, 0, 0);
	led.show();
};

void fcompassCalibrate(tstPrvMain *pstPrivate) 
/****************************************************************
Compass calibration
Run for every new location
*****************************************************************/
{
	tstCompass *pstCompass = &pstPrivate->stCompass;
	tstMotor *pstMotor = &pstPrivate->stMotor;

	bool			bStarted = false;
	signed int		iMinX = 0;
	signed int		iMaxX = 0;
	signed int		iMinY = 0;
	signed int		iMaxY = 0;
	unsigned long	ulStartTime = millis();

	
	pstMotor->bCalibRun = true;
	fMoveProcedure(pstMotor);

	for (unsigned int i = 0; (millis() - ulStartTime) < CALIB_TIME;)
	{
		if ((millis() - ulStartTime) > (CALIB_TIME / 2) && !bStarted)
		{
			bStarted = true;
			pstMotor->bCalibRunL = true;
			pstMotor->bCalibRun = false;
			fMoveProcedure(pstMotor);
		}
		mag.getHeading(&pstCompass->iMagnet_x, &pstCompass->iMagnet_y, &pstCompass->iMagnet_z);
		if (pstCompass->iMagnet_x < iMinX) iMinX = pstCompass->iMagnet_x; 
		if (pstCompass->iMagnet_x > iMaxX) iMaxX = pstCompass->iMagnet_x; 
		if (pstCompass->iMagnet_y < iMinY) iMinY = pstCompass->iMagnet_y; 
		if (pstCompass->iMagnet_y > iMaxY) iMaxY = pstCompass->iMagnet_y; 
		if (i < LCDCOLS && (millis() - ulStartTime) >= (CALIB_TIME / LCDCOLS * i))
		{
			lcd.setCursor(i, 1);
			lcd.write(0);
			i++;
		}
		fsetColor(pstPrivate);
	}

	// Calculate offsets
	pstCompass->iMagOffset_x = (iMaxX + iMinX) / 2; 
	pstCompass->iMagOffset_y = (iMaxY + iMinY) / 2; 

	pstMotor->bCalibRun = false;
	pstMotor->bCalibRunL = false;
	pstCompass->bCalibDone = true;
	pstMotor->bCompassCalibrated = pstCompass->bCalibDone;
	fMoveProcedure(pstMotor);
};

void fgetAngle(tstCompass *pstCompass)
/****************************************************************
Get actual angle from compass

*****************************************************************/
{
	float flTempAngle = 0.0;

	// Set declination angle on your location and fix heading
	// You can find your declination on: http://magnetic-declination.com/
	// (+) Positive or (-) for negative
	// Formula: (deg + (min / 60.0)) / (180 / M_PI);
	pstCompass->flDeclinationAngle = (2.0 + (16.0 / 60.0)) / (180.0 / M_PI);

	for (int i = 1; i <= SAMPLES; i++) {
		// Read raw heading measurements from device
		mag.getHeading(&pstCompass->iMagnet_x, &pstCompass->iMagnet_y, &pstCompass->iMagnet_z);
		pstCompass->iMagnet_x = pstCompass->iMagnet_x - pstCompass->iMagOffset_x;
		pstCompass->iMagnet_y = pstCompass->iMagnet_y - pstCompass->iMagOffset_y;

	   // To calculate heading in degrees. 0 degree indicates North
		flTempAngle += (atan2((float)pstCompass->iMagnet_y, (float)pstCompass->iMagnet_x) + pstCompass->flDeclinationAngle);
		
	}
	// Filtering angles
	flTempAngle = flTempAngle / SAMPLES;

	if (flTempAngle < 0)  flTempAngle += 2 * M_PI; 

	flTempAngle = flTempAngle * 180 / M_PI;

	pstCompass->uiAngle = (unsigned int)flTempAngle;
};

void fsetTone(tstPrvMain *pstBuzzer)
/****************************************************************
Generate and set a tone on the buzzer

*****************************************************************/
{
	signed int iAngleDiff = *pstBuzzer->stMotor.puiActAngle;
	iAngleDiff = abs(iAngleDiff - 180);
	
	if (iAngleDiff < 30)								tone(TONEPIN, TONEFREQ);
	else if(iAngleDiff >= 30 && iAngleDiff < 60)		tone(TONEPIN, 2 * TONEFREQ);
	else if (iAngleDiff >= 60 && iAngleDiff < 90)		tone(TONEPIN, 4 * TONEFREQ);
	else if (iAngleDiff >= 90 && iAngleDiff < 120)		tone(TONEPIN, 8 * TONEFREQ);
	else if (iAngleDiff >= 120 && iAngleDiff < 150)		tone(TONEPIN, 10 * TONEFREQ);
	else if (iAngleDiff >= 150)							tone(TONEPIN, 20 * TONEFREQ);
	else noTone(TONEPIN);
};

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

void fUIProcedure(tstPrvMain *pstPrivate)
/****************************************************************
Complete User Interface procedure

*****************************************************************/
{
	tstCompass *pstCompass = &pstPrivate->stCompass;
	tstMotor *pstMotor = &pstPrivate->stMotor;
	tstUI *pstUIProcedure = &pstPrivate->stUI;
	unsigned short		usKeyState;

	usKeyState = fgetKeyValue(pstUIProcedure);

	switch (usKeyState)
	{
	case enKey_1:

		if (!pstCompass->bCalibDone)
		{
			pstUIProcedure->enUIState = enUIState_Calibration;
			fsetUIMenu(pstPrivate);
		}
		else if (!pstUIProcedure->bStartAuto)
		{
			pstUIProcedure->bStartManual = true;
			pstUIProcedure->enUIState = enUIState_ManualMode;
			fsetUIMenu(pstPrivate);
		}
		break;

	case enKey_2:
		if (pstCompass->bCalibDone && !pstUIProcedure->bStartManual)
		{
			pstUIProcedure->bStartAuto = true;
			pstMotor->bRun = true;
			pstUIProcedure->enUIState = enUIState_AutomaticMode;
			fsetUIMenu(pstPrivate);
		}
		break;

	case enKey_3:
		if(pstCompass->bCalibDone)
		{
			pstUIProcedure->bStartAuto = false;
			pstUIProcedure->bStartManual = false;
			pstMotor->bRun = false;
			pstUIProcedure->enUIState = enUIState_Abort;
			fsetUIMenu(pstPrivate);
		}
		break;

	case enKey_undef:
		pstUIProcedure->enUIState = enUIState_undef;
		if (!pstUIProcedure->bMenuSet)	fsetUIMenu(pstPrivate);
		break;
	default:
		break;
	}
};

void fsetUIMenu(tstPrvMain *pstPrivate)
/****************************************************************
Indicate different User Interface menus

*****************************************************************/
{
	tstCompass *pstCompass = &pstPrivate->stCompass;
	tstMotor *pstMotor = &pstPrivate->stMotor;

	if (pstPrivate->stUI.enUIState == enUIState_Calibration)
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		if (!pstCompass->bCalibDone)		lcd.println("Compass calibr..");
		fcompassCalibrate(pstPrivate);
		lcd.setCursor(0, 0);
		lcd.print("                  ");
		lcd.setCursor(4, 0);
		lcd.print("Complete!");
		delay(2000);
		pstPrivate->stUI.bMenuSet = false;
	}
	else if (pstPrivate->stUI.enUIState == enUIState_undef && !pstCompass->bCalibDone)	
	{
		lcd.clear();
		lcd.println("PRESS Key 1 to  ");
		lcd.setCursor(0, 1);
		lcd.println("calib. compass! ");
		pstPrivate->stUI.bMenuSet = true;
	}
	else if(pstPrivate->stUI.enUIState == enUIState_undef && pstCompass->bCalibDone)
	{
		lcd.clear();
		lcd.println("Key 1 - ManMode ");
		lcd.setCursor(0, 1);
		lcd.println("Key 2 - AutoMode");
		pstPrivate->stUI.usPrevState = enUIState_undef;
		pstPrivate->stUI.bMenuSet = true;
	}
	else if (pstPrivate->stUI.enUIState == enUIState_ManualMode)
	{
		pstPrivate->stUI.usPrevState = enUIState_ManualMode;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("M:Act. Ang:   ");
		lcd.setCursor(15, 0);
		lcd.write(1);
		lcd.setCursor(0, 1);
		lcd.print("Key 3 - STOP");
	}
	else if (pstPrivate->stUI.enUIState == enUIState_AutomaticMode)
	{
		pstPrivate->stUI.usPrevState = enUIState_AutomaticMode;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("A:Act. Ang:   ");
		lcd.setCursor(15, 0);
		lcd.write(1);
		lcd.setCursor(0, 1);
		lcd.print("Key 3 - STOP");
	}
	else if ((pstPrivate->stUI.enUIState == enUIState_Abort) && pstPrivate->stUI.usPrevState > 0)
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		fMoveProcedure(pstMotor);
		lcd.print("Mode aborted!");
		delay(2000);
		pstPrivate->stUI.bMenuSet = false;
	}
};

void fUpdateDisplay(tstPrvMain *pstPrivate)
/****************************************************************
Update LCD Display

*****************************************************************/
{
	// Only for timing measurements
	//if (pstDisplay->stUI.ulCycle == 0)	pstDisplay->stUI.ulOldTime = micros();

	if (pstPrivate->stUI.ulCycle % 10 == 0)
	{
		pstPrivate->stUI.ulTime = micros();
		lcd.setCursor(12, 0);
		lcd.print("   ");
		lcd.setCursor(12, 0);
		lcd.print(*pstPrivate->stMotor.puiActAngle);

		// Only for timing measurements
		//Serial.print("Time diff: ");
		//Serial.println(pstDisplay->stUI.ulTime - pstDisplay->stUI.ulOldTime);
		//pstDisplay->stUI.ulOldTime = pstDisplay->stUI.ulTime;
	}
	pstPrivate->stUI.ulCycle++;
};