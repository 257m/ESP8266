extern "C" {
  #include "uart_io.h"
  #include "serial_io.h"
  
  #include "Wire.h"
  #include "BMP180I2C.h"
}

#define DEBUG 1

#define I2C_ADDRESS 0x77

#define fieldElevation 880 // Cambridge elevation
#define pressureOffSet 0.89 // Sensor offset

typedef struct {
	int x, y;
} Message;

Message joy = {0, 0};

// Left motor
#define enA 8
#define in1A 7
#define in2A 6

// Right motor
#define enB A2
#define in1B 9
#define in2B 10

#define vrx A0
#define vry A1
#define sw 11

const int DEADZONE = 10;

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
  
  
  Serial3.begin(9600);
  uart_select(3);
  pinMode(enA, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  
  pinMode(enB, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  
  pinMode(vrx, INPUT);
  pinMode(vry, INPUT);
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
  
	pressure = bmp180.getPressure()+pressureOffSet*100*33.864;

	Serial.print("Pressure: "); 
	Serial.print(pressure);
	Serial.print(" Pa | ");
	Serial.print(pressure/33.864/100);
	Serial.println(" inHg");
	
	
	
	pressureAltitude = ((pressure/33.864/100) - 29.92) * 1000 + fieldElevation;
	
	Serial.print("Pressure Altitude: ");
	Serial.print(pressureAltitude);
	Serial.println(" ft");



	densityAltitude = pressureAltitude + (120 * (temperature - (15 - fieldElevation/1000*2)));
	Serial.print("Density Altitude:");
	Serial.print(densityAltitude);
	Serial.println(" ft\n\n");
  
  
  
  if (uart_available())
    uart_memcpy((unsigned char*)&joy, sizeof(joy));
  joy.x = (joy.x/2)-255;
  joy.y = (joy.y/2)-255;

  setMotor(enA, in1A, in2A, joy.x);
  setMotor(enB, in1B, in2B, joy.y);
#if DEBUG
  Serial.print(joy.x);
  Serial.print("  ");
  Serial.println(joy.y);
#endif /* DEBUG */
}


void setMotor(int en, int in1, int in2, int speed) {
  digitalWrite(in1, speed < 0);
  digitalWrite(in2, speed > 0);
  
  analogWrite(en, abs(speed));
}
