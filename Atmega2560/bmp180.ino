
#include <Arduino.h>
#include <Wire.h>

#include <BMP180I2C.h>

#define I2C_ADDRESS 0x77

#define fieldElevation 880 // Cambridge elevation

BMP180I2C bmp180(I2C_ADDRESS);

double pressure;
double temperature;
double pressureAltitude;
double densityAltitude;

void setup() {
	Serial.begin(9600);

	while (!Serial);

	Wire.begin();

	if (!bmp180.begin())
	{
		Serial.println("begin() failed");
		while (1);
	}

	bmp180.resetToDefaults();

	bmp180.setSamplingMode(BMP180MI::MODE_UHR);
}

void loop() {
	delay(1000);

	if (!bmp180.measureTemperature())
	{
		Serial.println("Could not start temperature measurement");
		return;
	}

	while (!bmp180.hasValue());

  temperature = bmp180.getTemperature();

	Serial.print("Temperature: "); 
	Serial.print(temperature); 
	Serial.println(" degC");



  // Always use right after temperature measurement
	if (!bmp180.measurePressure())
	{
		Serial.println("Could not start pressure measurement");
		return;
	}

	while (!bmp180.hasValue());
  
  pressure = bmp180.getPressure();
  
	Serial.print("Pressure: "); 
	Serial.print(pressure);
	Serial.print(" Pa | ");
	Serial.print(pressure/33.864);
	Serial.println(" inHg");
	
	
	
	pressureAltitude = (pressure/33.864 - 29.92) * 1000 + fieldElevation;
	
	Serial.print("Pressure Altitude: ");
	Serial.print(pressureAltitude);
	Serial.print(" ft");
	
	
	
	densityAltitude = pressureAltitude + (120 * (temperature – (15 - fieldElevation/1000*2)));
	Serial.print("Density Altitude: ");
	Serial.print(densityAltitude);
	Serial.print(" ft");
}
