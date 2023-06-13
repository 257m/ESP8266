// Left motor
int speedA = 0;
#define enA 8
#define in1A 7
#define in2A 6

// Right motor
int speedB = 0;
#define enB A2
#define in1B 9
#define in2B 10

#define vrx A0
#define vry A1
#define sw 11

const int DEADZONE = 10;

void setup() {
  Serial.begin(9600);
  
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
  delay(25);
  speedA = abs(analogRead(vry)) >= DEADZONE ? analogRead(vrx)/2-255 : speedA;
  speedB = abs(analogRead(vrx)) >= DEADZONE ? analogRead(vry)/2-255 : speedB;
  
  setMotor(enA, in1A, in2A, speedA);
  setMotor(enB, in1B, in2B, speedB);
  Serial.print(speedA);
  Serial.print("  ");
  Serial.println(speedB);
}


void setMotor(int en, int in1, int in2, int speed) {
  digitalWrite(in1, speed < 0);
  digitalWrite(in2, speed > 0);
  
  analogWrite(en, abs(speed));
}
