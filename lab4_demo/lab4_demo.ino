//demo for lab4
//ECE 520.340 Intro to Mechatronics
//Yichuan Tang
#define BlueLED 5
#define YellowLED 6
#define Flex A0
#define Ping 7
float VCC = 5.0;
float R_DIV = 10000.0;
float Straight_Resistance = 37300.0;
float Bend_Resistance = 90000.0;

void setup() {
  Serial.begin(9600);
  pinMode(Flex, INPUT);
}

void loop() {
  //Trigger and read ping sensor
  unsigned long duration, inches, cm;
  pinMode(Ping, OUTPUT);
  digitalWrite(Ping, LOW);
  delayMicroseconds(2);
  digitalWrite(Ping, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping, LOW);
  pinMode(Ping, INPUT);
  duration = pulseIn(Ping, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  //Light up Blue LED
  map((int)cm,
  analogWrite(BlueLED, cm);
  //Read flex sensor
  int FlexADC = analogRead(Flex);
  float FlexV = FlexADC * VCC /1023.0;
  float FlexR = R_DIV * (VCC / FlexV - 1.0);
  float angle = map(FlexR, Straight_Resistance, Bend_Resistance, 0, 90.0)
  //Light up Yellow LED
  analogWrite(YellowLED, angle)
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 29 / 2;
}

