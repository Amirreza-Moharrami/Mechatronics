#include<TimerOne.h>

// Defining Pins
const int encoderPinA=2;
const int encoderPinB=3;
const int motorPinIN1=4;
const int motorPinIN2=5;
const int motorPinPWM=11;

// Controller's Parameters
const float alpha = 0.099982 ;
const float beta = 0.2519;
float e1 = 0;
float e0 = 0;
float controlEffort = 0;
float ref = 1000.0;

//flag & counter
int counterPrint=0;
int flag = 1;

//Sampling frequency
float f = 100; // T=0.01 s

//Nummber of pulses produced by the encoder within a revolution.
float PPR = 100.0; //Ne

//Recording the current number of pulses received
int currentPosition = 0;

//Velocity in RPM
float velocity;

//Setup-----------------------------------------------------------------------------------------------------

void setup() 
{
  //Motor
  pinMode(motorPinIN1,OUTPUT);
  pinMode(motorPinIN2,OUTPUT);
  pinMode(motorPinPWM,OUTPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA),doEncoder,RISING); //Hardware 
  Interrupt with pin A
  Serial.begin(115200);

  //Setting the desired frequency
  Timer1.initialize(1000000/f); // T= 0.01 second
  Timer1.attachInterrupt(velocityMeasure); //Software Interrupt

  digitalWrite(motorPinIN1,HIGH);
  digitalWrite(motorPinIN2,LOW);
  analogWrite(motorPinPWM,200);
}

//Loop------------------------------------------------------------------------------------------------------

void loop() 
{
  if((flag==1) )
  {
    Serial.println(velocity); 
    flag=0;
  }
}

//Software Interrupt Function--------------------------------------------------------------------------

void velocityMeasure(void)
{
  flag=1;
  velocity = 60.0*(currentPosition)/(PPR/f);
  counterPrint++; 

  // controller
  e0 = ref - velocity; 
  controlEffort = controlEffort + alpha*e0 - alpha*beta*e1;

  if(controlEffort > 255)
  {
    controlEffort = 255;
  }
  if(controlEffort < 0)
  {
    controlEffort = 0;
  }

  //updating e0 for the next call
  e1 = e0;
  analogWrite(motorPinPWM,controlEffort);
  currentPosition = 0; 
}

//Hardware Interrupt Function-------------------------------------------------------------------------

void doEncoder()
{
if(digitalRead(encoderPinB)== LOW)
  {
    currentPosition++;
  }
  
  else
  {
    currentPosition--;
  }
}
