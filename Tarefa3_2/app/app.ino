#include "event_driven.h"
#include "app.h"
#include "pindefs.h"

bool state = false;
unsigned long timee = 1000;
int lastButtonPressed = -1;
unsigned long lastpress;
void appinit(void)
{
  //Dois botões serão observados
  button_listen(BUT1);
  button_listen(BUT2);
}
void button_changed(int p, int v) 
{
  if(lastButtonPressed != -1 && p != lastButtonPressed && (millis() - lastpress) < 600)
  {
     Serial.println(lastButtonPressed);
     Serial.println(p);
     Serial.println("Passou3");
     digitalWrite(LED1, LOW);
     while(1);
  }
  else if (p == BUT1 && v == LOW)
  {
    if(timee > 0)
    {
         timee -= 300; 
         Serial.println("Passou1");  
         lastpress = millis();
    }
  }
  else if (p == BUT2 && v == LOW)
  {
    timee += 300;
    Serial.println("Passou2");
    lastpress = millis();
  }
  lastButtonPressed = p;
  Serial.println("Passou pela função");
}
void timer_expired(void)
{
  state = !state;
  digitalWrite(LED1, state);
  timer_set(timee);
}
