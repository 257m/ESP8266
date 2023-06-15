extern "C" {
  #include "uart_io.h"
  #include "serial_io.h"
}

#define DEBUG 1

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

void setup() {
  Serial.begin(9600);
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
