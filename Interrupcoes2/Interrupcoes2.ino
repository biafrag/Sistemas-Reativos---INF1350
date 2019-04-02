#include "pindefs.h"
#include <avr/sleep.h>
#include <avr/power.h>
byte state = HIGH;

volatile int counter = 0;

volatile int buttonChanged = 0;
int ID = 0;
int count = 0;

void pciSetup (byte pin) 
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interruptjk
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void disable (byte pin)
{
  *digitalPinToPCMSK(pin) &= ~bit (digitalPinToPCMSKbit(pin)); 
}

ISR (PCINT1_vect) { // handle pin change interrupt for A0 to A5 here
   buttonChanged=1;
 }  

void timerSetup () {
   TIMSK2 = (TIMSK2 & B11111110) | 0x01;
   TCCR2B = (TCCR2B & B11111000) | 0x07;
}


void setup() {
   pinMode(LED1, OUTPUT); digitalWrite(LED1, state);
   pinMode(LED2, OUTPUT); digitalWrite(LED2, state);
   pinMode(LED3, OUTPUT); digitalWrite(LED3, state);
   pinMode(LED4, OUTPUT); digitalWrite(LED4, state);
   pinMode (KEY1, INPUT_PULLUP); 
   pinMode (KEY2, INPUT_PULLUP);
   pinMode(KEY3, INPUT_PULLUP);
   timerSetup();
   pciSetup(KEY1); pciSetup(KEY2); pciSetup(KEY3);
   Serial.begin(9600); 
}
 
void loop() {
  if (counter>50) 
  {
    state = !state;
    digitalWrite(LED1, state);
    counter = 0;
  }
  if (buttonChanged) 
  {
    if(digitalRead(KEY1) == HIGH && (millis() - count) < 500)
    {
      if(digitalRead(KEY2) == LOW)
      {
        count = millis();
        digitalWrite(LED1,LOW);
        while(1);
      }
    }
    if (digitalRead(KEY2) == HIGH && (millis() - count) < 500)
    {
      if(digitalRead(KEY1) == LOW)
      {
        digitalWrite(LED1,LOW);
        count = millis();
        while(1);
      }
    }
    if(digitalRead(KEY2) == LOW && ID!=2)
    {
      ID = 2;
      count = millis();
    }
    if(digitalRead(KEY1) == LOW && ID!=1)
    {
      ID = 1;
      count = millis();
    }
     
     buttonChanged = 0;
  }
}

 
ISR(TIMER2_OVF_vect){
   counter++;
}
