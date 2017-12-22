// 
// 
// 

#include "test.h"
#include "HMC5883L.h"
#include <math.h>

HMC5883L mag;
int16_t mx, my, mz;

void initCompass() {
	Serial.println("Initializing I2C devices...");
	mag.initialize();
	Serial.println("Testing device connections...");
	Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
}

int calculateSpeed(int angle) {
	// y = m * x + b
	MIN_V = 40; // Define
	MAX_V = 100;// Define
	return (MAX_V - MIN_V) / 180 * abs(angle-180) + MIN_V;
}

int getMeasurement() {
	float heading= 0.0;
	int samples = 10;

	// Set declination angle on your location and fix heading
	// You can find your declination on: http://magnetic-declination.com/
	// (+) Positive or (-) for negative
	// Formula: (deg + (min / 60.0)) / (180 / M_PI);
	float declinationAngle = (2.0 + (16.0 / 60.0)) / (180.0 / M_PI);

	for (int i = 1; i <= samples; i++) {
		// read raw heading measurements from device
		mag.getHeading(&mx, &my, &mz);

		//mx = mx - (-326); //Offset Brugg Projektraum
		//my = my - 288; //Offset Brugg Proejktraum
		mx = mx - 291; //Offset Birmi
		my = my - 321; //Offset Birmi

		/*
		// display tab-separated gyro x/y/z values
		Serial.print("mag:\t");
		Serial.print(mx); Serial.print("\t");
		Serial.print(my); Serial.print("\t");
		Serial.print(mz); Serial.print("\t");
		*/

		// To calculate heading in degrees. 0 degree indicates North
		heading += (atan2(my, mx) + declinationAngle);
	}
	heading = heading / samples;

	if (heading < 0) { heading += 2 * M_PI; }
	heading = heading * 180 / M_PI;
	
	return round(heading);
}

/*****************************************************************/
void fMoveProcedure(int angle)
/****************************************************************
Move procedure of a motor with arguments of direction and speed

*****************************************************************/
{
	int angle_max= 355;
	int angle_min= 5;
	int speed = calculateSpeed(angle);
	if (angle <= angle_min || angle >= angle_max)
	{
		analogWrite(SpeedPinRight, LOW);
		analogWrite(SpeedPinLeft, LOW);
	}
	if (angle < angle_max && angle >= 180)
	{
		// Rechtsdrehen
		digitalWrite(DirectionRight, FORW);
		analogWrite(SpeedPinRight, speed);
		delay(2);
		digitalWrite(DirectionLeft, BACK);
		analogWrite(SpeedPinLeft, speed);
	}
	else
	{
		// Linksdrehen
		digitalWrite(DirectionRight, BACK);
		analogWrite(SpeedPinRight, speed);
		delay(2);
		digitalWrite(DirectionLeft, FORW);
		analogWrite(SpeedPinLeft, speed);
	}


};