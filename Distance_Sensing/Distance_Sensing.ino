int LEDpin = 9; //or 9 for LED light component
int IRpin = A1;
void setup() {
  // put your setup code here, to run once:
  pinMode (LEDpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int IRvalue = analogRead(IRpin);
  /*if(IRvalue > 300){
    digitalWrite(LEDpin, HIGH);
  }
  else{
    digitalWrite(LEDpin, LOW);
  }*/
  analogWrite(LEDpin, IRvalue/10); //for LED light component
  Serial.print("IR sensor value: ");
  Serial.println(IRvalue);
}
