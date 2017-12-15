// test.h

#ifndef _TEST_h
#define _TEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

void initCompass(void);
float getMeasurement(void);

#endif




