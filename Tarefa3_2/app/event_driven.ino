#include "event_driven.h"
#include "app.h"
#include "pindefs.h"

bool debounce[3] = {true,true,true};
bool lastState[3] = {HIGH,HIGH,HIGH};
float timeDebounce[3] = {600,600,600};
bool buttonsUsed[3]= {false,false,false}; 
bool stateButton = false;
int buttons[3] = {BUT1,BUT2,BUT3};
unsigned long old = 0;
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
  Serial.begin(9600);
  appinit();
}

bool bouncing(float timeDebounce)
{
    if(millis() - timeDebounce >= 500)
  {
    return true;
  }
  return false;
}

void loop()
{
  for(int i = 0; i < 3; i++)
  {
    debounce[i] = bouncing(timeDebounce[i]);
    if(debounce[i])
    {
        stateButton = digitalRead(buttons[i]);
        if(buttonsUsed[i] && stateButton != lastState[i])
        {
             button_changed(buttons[i],stateButton) ; 
             timeDebounce[i] = millis();
             debounce[i] = false;   
        } 
    }
    
  }
  unsigned long now = millis();
  if(now >= old+timee)
  {
      old = now;
      timer_expired();
  }
}
