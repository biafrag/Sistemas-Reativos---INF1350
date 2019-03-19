#include "event_driven.h"
#include "app.h"
#include "pindefs.h"

void appinit(void)
{
  button_listen(BUT1);
}
void button_changed(int p, int v) 
{
  digitalWrite(LED1, v);
}
void timer_expired(void)
{
  
}
