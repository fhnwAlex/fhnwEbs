#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;// Store our compass as a variable.

void setup()
{
  Serial.begin(9600);// Initialize the serial port.
  Wire.begin();// Start the I2C interface.
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);// Set the measurement mode to Continuous
}
void loop()
{
  MagnetometerRaw raw = compass.ReadRawAxis();// Retrive the raw values from the compass
  MagnetometerScaled scaled = compass.ReadScaledAxis();// Retrived the scaled values from the compass (scaled to the configured scale).
  float xHeading = atan2(scaled.YAxis, scaled.XAxis);
  float yHeading = atan2(scaled.ZAxis, scaled.XAxis);
  float zHeading = atan2(scaled.ZAxis, scaled.YAxis);
  if(xHeading < 0)    xHeading += 2*PI;
  if(xHeading > 2*PI)    xHeading -= 2*PI;
  if(yHeading < 0)    yHeading += 2*PI;
  if(yHeading > 2*PI)    yHeading -= 2*PI;
  if(zHeading < 0)    zHeading += 2*PI;
  if(zHeading > 2*PI)    zHeading -= 2*PI;
  float xDegrees = xHeading * 180/M_PI; 
  float yDegrees = yHeading * 180/M_PI; 
  float zDegrees = zHeading * 180/M_PI; 
  xDegrees = 0.0011*xDegrees*xDegrees + 0.5746*xDegrees + 17.011;
  Serial.print(xDegrees);
  Serial.print(",");
  Serial.print(yDegrees);
  Serial.print(",");
  Serial.print(zDegrees);
  Serial.println(";");
  delay(100);
}





