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
