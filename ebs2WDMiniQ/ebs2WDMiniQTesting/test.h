// test.h

#ifndef _TEST_h
#define _TEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

void initCompass(void);
int getMeasurement(void);
int calculateSpeed(int angle);
void fMoveProcedure(int angle);

#endif




