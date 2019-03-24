#include "event_driven.h"
#include "app.h"
#include "pindefs.h"

bool buttonsUsed[3]= {false,false,false}; 
int state = HIGH;
int buttons[3] = {BUT1,BUT2,BUT3};

void button_listen (int pin)
{
  if(pin == BUT1)
  {
    buttonsUsed[0] = true;
  }
  else if (pin == BUT2)
  {
    buttonsUsed[1] = true;
  }
  else

  {
    buttonsUsed[2] = true;
  }
}

void timer_set (int ms)
{
  
}

void setup()
{
  pinMode(BUT1,INPUT_PULLUP);
  pinMode(BUT2,INPUT_PULLUP);
  pinMode(BUT3,INPUT_PULLUP);
  pinMode(LED1,OUTPUT);
  button_changed(BUT1,HIGH) ;
  appinit();
}

void loop()
{
  for(int i = 0; i < 3; i++)
  {
    state = digitalRead(buttons[i]);
    if(buttonsUsed[i])
    {
         button_changed(buttons[i],state) ; 
    }
    
  }
}
