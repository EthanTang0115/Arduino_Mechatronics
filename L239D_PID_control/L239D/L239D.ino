//This an experiment of using L239D H-bridge to control Pololu DC motor
#define InA        9
#define InB        8
#define PWM        10 //the enable pin of motor
#define encodA     2
#define encodB     3
#define LOOPTIME   100 //PID loop time
#define NUMREADINGS 5 //samples for Amp average

volatile long encodCount = 0;
int readings [NUMREADINGS];
unsigned long lastMilli = 0;
unsigned long lastMilliPrint = 0;
int voltage = 0;
int current = 0;
int actSpeed = 0;
int reqSpeed = 320;
int encoder0PinALast = LOW;
int n = LOW;
int PWM_val = 0;
float Kp = 0.5;
float Kd = 1.2;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(115200);
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(encodA, INPUT);
  pinMode(encodB, INPUT);
  attachInterrupt(1, encodReading, FALLING);
  for(int i=0; i<NUMREADINGS;i++) readings[i] = 0; //initilize readings to all zeros

  analogWrite(PWM, PWM_val);
  digitalWrite(encodA, LOW);
  digitalWrite(encodB, HIGH);
}

void loop() {
 getParam();                                                                 // check keyboard
 if((millis()-lastMilli) >= LOOPTIME)   {                                    // enter tmed loop
   lastMilli = millis();
   getMotorData();                                                           // calculate speed, volts and Amps
   PWM_val= updatePid(PWM_val, reqSpeed, actSpeed);                        // compute PWM value
   analogWrite(PWM, PWM_val);                                               // send PWM to motor
 }
 printMotorInfo();           
}

void minit() {
  analogWrite(PWM, 255);
  delay(1000);
}

void getMotorData()  {                                                        // calculate speed, volts and Amps
static long countAnt = 0;                                                   // last count
 actSpeed = ((encodCount - countAnt)*(60*(1000/LOOPTIME)))/(16*29);          // 16 pulses X 29 gear ratio = 464 counts per output shaft rev
 countAnt = encodCount;                  
 //voltage = int(analogRead(Vpin) * 3.22 * 12.2/2.2);                          // battery voltage: mV=ADC*3300/1024, voltage divider 10K+2K
 //current = int(analogRead(Apin) * 3.22 * .77 *(1000.0/132.0));               // motor current - output: 130mV per Amp
 //current = digital_smooth(current, readings);                                // remove signal noise
}

int updatePid(int command, int targetValue, int currentValue)   {             
float pidTerm = 0;                                                     
int error=0;                                  
static int last_error=0;                             
 error = abs(targetValue) - abs(currentValue); 
 pidTerm = (Kp * error) + (Kd * (error - last_error));                            
 last_error = error;
 return constrain(command + int(pidTerm), 0, 255);
}

void encodReading() {
  if (PINB & 0b00000001)    encodCount++;
  else                      encodCount--;
}

int getParam()  {
char param, cmd;
 if(!Serial.available())    return 0;
 delay(10);                  
 param = Serial.read();                              // get parameter byte
 if(!Serial.available())    return 0;
 cmd = Serial.read();                                // get command byte
 Serial.flush();
 switch (param) {
   case 'v':                                         // adjust speed
     if(cmd=='+')  {
       reqSpeed += 20;
       if(reqSpeed>400)   reqSpeed=400;
     }
     if(cmd=='-')    {
       reqSpeed -= 20;
       if(reqSpeed<0)   reqSpeed=0;
     }
     break;
   case 's':                                        // adjust direction
     if(cmd=='+'){
       digitalWrite(InA, LOW);
       digitalWrite(InB, HIGH);
     }
     if(cmd=='-')   {
       digitalWrite(InA, HIGH);
       digitalWrite(InB, LOW);
     }
     break;
   case 'o':                                        // user should type "oo"
     digitalWrite(InA, LOW);
     digitalWrite(InB, LOW);
     reqSpeed = 0;
     break;
   default: 
     Serial.println("???");
   }
}

void printMotorInfo()  {                                                      // display data
 if((millis()-lastMilliPrint) >= 500)   {                     
   lastMilliPrint = millis();
   Serial.print("ReqSpeed:");             Serial.println(reqSpeed);
   Serial.print("ActSpeed:");          Serial.println(actSpeed);
   Serial.print("PWM:");          Serial.println(PWM_val);  Serial.println(""); 
   //Serial.print("  V:");            Serial.print(float(voltage)/1000,1);
   //Serial.print("  mA:");           Serial.println(current);

   //if (current > CURRENT_LIMIT)               Serial.println("*** CURRENT_LIMIT ***");                
   //if (voltage > 1000 && voltage < LOW_BAT)   Serial.println("*** LOW_BAT ***");                
 }
}
