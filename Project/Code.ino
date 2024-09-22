#include<TimerOne.h>

float temp;

// Encoder
const int encoderPinA=2;
const int encoderPinB=3;
float PPR = 400.0; //Ne
float f = 100; // T=0.01 s
volatile float velCoef = 60.0/(PPR/f);
volatile float posCoef = 360.0/PPR;
volatile float velocity;
volatile float position;
int printCounter = 0;
int flag = 1;

//Solenoid
int solenoidPin = 11; //This is the output pin on the Arduino we are using

//This variable will increase or decrease depending on therotation of encoder
long int counter = 0, posCounter = 0, velCounter = 0, encPulses=0;


// Controller’s Parameters
const float alpha = 3 ; //alpha = 2.4 //alpha = 2.4 ;
const float beta = 0.3; //beta = 0.50 //beta = 0.69;
float e1 = 0;
float e0 = 0;
float controlEffort = 205;
float ref = 220.0;

//Setup -------------------------------------------------------------------------------------------------------------------------
void setup()
{
  // TCCR2B = TCCR2B & B11111000 | B00000101;
  Serial.begin (115200);

  //Encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  //Solenoid
  pinMode(solenoidPin, OUTPUT); //Sets the pin as an output
  attachInterrupt(digitalPinToInterrupt(encoderPinA),EncoderA, RISING); //Hardware Interrupt

  Timer1.initialize(1000000/f); // T= 0.01 second
  Timer1.attachInterrupt(kinematicsMeasure); //Software Interrupt
  // analogWrite(solenoidPin,205);
}

//Loop--------------------------------------------------------------------------------------------------------------------------

void loop()
{
  temp=analogRead(A0);
  temp=temp*5000/1023/10;

  if(flag == 1 && printCounter == 50 )
  {
    Serial.print("RPM:");
    Serial.print(velocity);
    Serial.print(" Temp:");
    Serial.println(temp);
    // Serial.println(position);
    // Serial.println(encPulses);
    flag=0;
    printCounter = 0;
  }

  if(temp >= 60.0)
  {
    analogWrite(solenoidPin, 0);
  }
}

//Encoder Measure------------------------------------------------------------------------------------------------------------

void EncoderA()
{
  if(digitalRead(encoderPinB)==LOW)
  {
    velCounter--;
    // posCounter--;
    encPulses--;
  }
  else
  {
    velCounter++;
    // posCounter++;
    encPulses++;
  }
}

//Kinematics Measure--------------------------------------------------------------------------------------------------------
void kinematicsMeasure()
{
  flag=1;
  printCounter++;
  velocity = velCoef*(velCounter);
  position = posCoef*(encPulses % 400);
  velCounter = 0 ;

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

  solenoidActuation();
}

//Solenoid Actuation--------------------------------------------------------------------------------------------------------

void solenoidActuation()
{
  if(((position >=0) && (position <=140)))
  {
    analogWrite(solenoidPin, controlEffort);
  }
  else
    analogWrite(solenoidPin, 0);
}