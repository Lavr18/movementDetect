/*
  This script stores a movement data from the HC-SR501 movement sensor in EEPROM memeory.
  For each detected movement, its minute, hour, and day are stored.
  Hardware: Arduino Uno, HC-SR501 sensor.
  By Aliaksei Laurynovich.
  22.03.2019.
*/

#include <EEPROM.h>

const int movementPin = 2;     // the number of the sensor pin

bool movementState = 0;         // variable for reading the movement status
short seconds = 0;              // counts up every 1 second
byte minutes = 0;               // counts up every 1 minute
byte hours = 0;                 // counts up every 1 hour
byte days = 0;                  // counts up every 1 day
short timer = 0;                // counts up every 1 ms
int address = 0;                // EEPROM address counter


void setup() 
{
/* Uncomemment when reading the stored data */  
//  //initialize the serial monitor to display the movement state
//  Serial.begin(9600);
//  Serial.println("Initialised");
//  Serial.println( "Compiled: " __DATE__ ", " __TIME__ ", " __VERSION__); 
// 
//  // initialize the movement digital pin as an input:
//  pinMode(movementPin, INPUT);
//
//   printTemp();

  // initialize a timer interrupt to give interrupts every 1 ms
   TCCR0A|=(1<<WGM01);    //Set the CTC mode
   OCR0A=0xF9;            //Set the value for 1ms
   TIMSK0|=(1<<OCIE0A);   //Set the interrupt request
   sei();                 //Enable interrupt
   TCCR0B|=(1<<CS01);     //Set the prescale 1/64 clock
   TCCR0B|=(1<<CS00);
}

void loop() 
{
  // manage a timer to count seonds, numutes, hours, and days
   if(timer >= 1000)
   {
    seconds++;
    timer = 0;
   }
   if(seconds >= 60)
   {
    minutes++;
    seconds = 0;
   }
   if(minutes >= 60)
   {
    hours++;
    minutes = 0;
   }
   if(hours >= 60)
   {
    hours++;
    minutes = 0;
   }
   
  // read the state of the movement value:
  movementState = digitalRead(movementPin);
  
/* Uncomment when reading data */
//  Serial.println(timer);
//  Serial.println(seconds);
//  Serial.println(minutes);

  // check if the movement is detected. If it is, write the time to EEPROM:
  if (movementState) 
  {
    writeTemp(minutes);
    writeTemp(hours);
    writeTemp(days);
  }
  
  delay(500);
}

// this is the interrupt request
ISR(TIMER0_COMPA_vect)
{    
  timer++;
}

// print all EEPROM values to the serial monitor
void printTemp()
{
  for (int i = 0 ; i < EEPROM.length() ; i += 3) //EEPROM.length()
  {
    
    for(int j = 0 ; j < 3 ; j++)                
    {
      byte value = EEPROM.read(i + j);            // read EEPROM data at address i + j (+0 for min, +1 for hour, +2 for day)              
      Serial.print(i/3 + 1);                      // print the movement number
      Serial.print(" ");
      Serial.print(j + 1);                        // print the data label
      Serial.print(" ");
      Serial.println(value);
    }
  }
}

// write minute, hour, and day of the movement to the next EEPROM address
void writeTemp(byte value)
{
  EEPROM.write(address, value);
  delay(5);                                       // give it some time to complete a write        
   
  address++;                                      // increment address counter
  if(address == EEPROM.length())                  // check if address counter has reached the end of EEPROM
  {
    address = 0;                                  // if yes: reset address counter
  }
}
