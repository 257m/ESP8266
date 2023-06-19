extern "C" {
  #include "uart_io.h"
  #include "serial_io.h"
}

#define DEBUG 0

#include "Wire.h"
#include "BMP180I2C.h"

#define I2C_ADDRESS 0x77

#define fieldElevation 880 // Cambridge elevation
#define pressureOffSet 0.89 // Sensor offset

// Structures for joystick data and sensor reading
// These have to be type long and double because
// atmega 2560 is 8 bit while esp8266 is 32 bit
typedef struct {
	long x, y;
} Message;

typedef struct {
	double temperature, pressure, pressureAltitude, densityAltitude;
	unsigned int ir_cloud, sound;
} Sensor_Reading;

Message joy = {0, 0};

Sensor_Reading sr = {1.5f, 1.5f, 1.5f, 1.5f};

// Left motor
#define enA 8
#define in1A 7
#define in2A 6

// Right motor
#define enB 12
#define in1B 11 
#define in2B 10

// const int DEADZONE = 10;
BMP180I2C bmp180(I2C_ADDRESS);

void setup() {
	Serial.begin(9600);
	pinMode(enA, OUTPUT);
	pinMode(in1A, OUTPUT);
	pinMode(in2A, OUTPUT);

	pinMode(enB, OUTPUT);
	pinMode(in1B, OUTPUT);
	pinMode(in2B, OUTPUT);

	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	Wire.begin();

	if (!bmp180.begin() && DEBUG)  {
		Serial.println("begin() failed");
	}

	bmp180.resetToDefaults();

	bmp180.setSamplingMode(BMP180MI::MODE_UHR);
}

void loop()
{
	while (!Serial.available());
	// If there is then check if it a one or zero
	if (Serial.read()) {
		// Measure readings with sensors
		bmp180.measureTemperature();
		while (!bmp180.hasValue());
		sr.temperature = bmp180.getTemperature();
#if DEBUG
		Serial.print("Temperature: "); 
		Serial.print(sr.temperature); 
		Serial.println(" degC");
#endif /* DEBUG */
		// Always use right after temperature measurement
		bmp180.measurePressure();
		while (!bmp180.hasValue());
		sr.pressure = bmp180.getPressure()+pressureOffSet*100*33.864;
#if DEBUG
		Serial.print("Pressure: "); 
		Serial.print(sr.pressure);
		Serial.print(" Pa | ");
		Serial.print(sr.pressure/33.864/100);
		Serial.println(" inHg");
#endif /* DEBUG */
		sr.pressureAltitude = ((sr.pressure/33.864/100) - 29.92) * 1000 + fieldElevation;
#if DEBUG
		Serial.print("Pressure Altitude: ");
		Serial.print(sr.pressureAltitude);
		Serial.println(" ft");
#endif
		sr.densityAltitude = sr.pressureAltitude + (120 * (sr.temperature - (15 - fieldElevation/1000*2)));
#if DEBUG
		Serial.print("Density Altitude:");
		Serial.print(sr.densityAltitude);
		Serial.println(" ft\n\n");
#endif /* DEBUG */
		sr.ir_cloud = analogRead(A0);
		sr.sound = analogRead(A1);
		// If it is a one send the sensor reading
		Serial.write((unsigned char*)&sr, sizeof(Sensor_Reading));
	}
	else {
		// If it is a zero read the joystick data
		Serial.readBytes((unsigned char*)&joy, sizeof(joy));
		// Map joystick data to motor speeds
		joy.x = (joy.x/2)-255;
		joy.y = (joy.y/2)-255;
		// Set the motor to joystick speeds
		setMotor(enA, in1A, in2A, joy.x);
		setMotor(enB, in1B, in2B, joy.y);
#if DEBUG
		Serial.print(joy.x);
		Serial.print("  ");
		Serial.println(joy.y);
#endif /* DEBUG */
	}
}

void setMotor(int en, int in1, int in2, int speed) {
	digitalWrite(in1, speed < 0);
	digitalWrite(in2, speed > 0);

	analogWrite(en, abs(speed));
}
