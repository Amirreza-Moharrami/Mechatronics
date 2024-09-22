#include <TimerOne.h>
#define pin A0 // Analog pin connected to potentiometer
#define frequency 2 // frequency of data reading
float voltage =0; 
int flag = 0; // flag == 1 means data is read.

void setup() 
{
  Serial.begin(9600); 
  // reads the voltage with a frequency of 2Hz 
  Timer1.initialize(1000000/frequency);
  // defining interrupt
  Timer1.attachInterrupt(Interrupt);
}


void loop() 
{
  // prints the output in serial monitor
  if (flag)
  {
    Serial.print("Voltage(V): ");
    Serial.println(voltage/1023.0 * 5.0);
    flag = 0;
  }
}

// ISR
void Interrupt()
{
  flag = 1;
  voltage = analogRead(pin);
}