#define interruptPin 2
#define mode FALLING

//Define flags and time variables
int flag = 0; //flag==1 means:start since the first push
int flagDouble = 0; //flagDouble==1 means:Double click in at a short time
long int clickTime = 0;
long int reportTime = 0;
long int previousTime = 0;
long int previousClickTime=0;
long int firstTime=0;
int clickCounter;

void setup() 
{
  Serial.begin(9600);
  pinMode(interruptPin,INPUT_PULLUP); //Define Interrupt Pin
  attachInterrupt(digitalPinToInterrupt(interruptPin)click,mode);

}

void loop() 
{

  //Start Mode
  if( clickCounter % 2 == 0 )
  {
    firstTime=millis();
    previousTime=reportTime; 
  }

  //Stop Mode
  if( clickCounter % 2 == 1 )
  {
    reportTime = millis() - firstTime + previousTime;
  }

  if (flag == 1)
  {
    //Reset Mode 
    if(flagDouble==1)
    {
      firstTime=clickTime;
      clickCounter = 0 ;
      previousTime = 0;
      reportTime = 0;
      flagDouble=0;
    }

    //Print in Serial Monitor
    Serial.print("Result Time (ms): ");
    Serial.println( reportTime );
  }
}

//Interrupt Function
void click()
{
  previousClickTime=clickTime;
  clickTime = millis();
  //Check Debouncing

  if (abs(clickTime-previousClickTime) > 250)
  {
    flag = 1;
    clickCounter += 1;

    if (abs(clickTime-previousClickTime) < 1000 )
    {
      flagDouble=1;
    } 
  }
}
