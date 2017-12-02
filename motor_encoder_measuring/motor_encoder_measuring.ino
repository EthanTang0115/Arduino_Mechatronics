#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

int val;
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int loopcounter = 2;
int n = LOW;
bool forward = false;
int i = 70;
  
void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.print("Running Motor With Encoder Testing:");
  Serial.print("");

  //AFMS.begin();  // create with the default frequency 1.6KHz
  AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
}

void loop() {
// case 1: forward to 450 encoder reading
if (loopcounter ==1)
{
    myMotor->run(FORWARD);
    myMotor->setSpeed(i);
   n = digitalRead(encoder0PinA);
  if (-5 <= encoder0Pos && encoder0Pos < 450)
  {
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    Serial.print ("Encoder:");Serial.println (encoder0Pos);Serial.println("fw");
  }
  }

  if (encoder0Pos == 450)
  {
  myMotor->run(RELEASE);
  delay(500);
  Serial.print("final speed:");Serial.println(i);Serial.println ("");
  loopcounter = 2;
  delay(3000);
  }
  encoder0PinALast = n;
}

// case 2: backward to 0 encoder reading
encoder0PinALast = LOW;
if (loopcounter == 2)
{
    myMotor->run(BACKWARD);
    myMotor->setSpeed(i);
 if (encoder0Pos >= 0)
 {
  n = digitalRead(encoder0PinA);
   if (450 >= encoder0Pos > 0)
   {
   if ((encoder0PinALast == LOW) && (n == HIGH)) 
   {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    Serial.print("Encoder:");Serial.println (encoder0Pos);
  }
  }
 }

  if (encoder0Pos == 0)
  {
  myMotor->run(RELEASE);
  delay(500);
  Serial.print("final speed:");Serial.println(i);Serial.println ("");
  loopcounter = 1;
  delay(3000);
  }
  encoder0PinALast = n;
}
  
}
