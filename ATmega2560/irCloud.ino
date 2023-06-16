#define IRPin 15
int val;
String text;

void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(115200);
}

void loop() {
  val = analogRead(IRPin);
  
  if (val < 160) {
    text = "Overcast/broken cloud layer.";
  }
  else if (val < 200) {
    text = "Scattered cloud layer."
  }
  else {
    text = "Clear sunny skies!"
  }
  
  Serial.println(text);
}
