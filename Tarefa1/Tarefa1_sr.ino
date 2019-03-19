#define LED1 13
#define BUT1 A1
#define BUT2 A2
#define BUT3 A3

bool debounce[3] = {true,true,true};
float timeDebounce[3] = {600,600,600};
unsigned long old = 0;
int timee = 1000;
bool state = true;

void setup() {
  // iNICIALIZANDO
  pinMode(LED1, OUTPUT);
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  Serial.begin(9600);
}

bool bouncing(float timeDebounce)
{
    if(millis() - timeDebounce >= 500)
  {
    return true;
  }
  return false;
}
void loop() {
  for(int i = 0; i < 3 ; i++)
  {
    debounce[i] = bouncing(timeDebounce[i]);
  }
  if(digitalRead(BUT1) == LOW && debounce[0] )
  {
     if(timee > 0)
    {
         timee -= 100; 
    }
    Serial.print("Passou");
    timeDebounce[0] = millis();
    debounce[0] = false;
  }
  if(digitalRead(BUT2) == LOW && debounce[1] )
  {

    timee += 100; 
    Serial.print("Passou");
    timeDebounce[1] = millis();
    debounce[1] = false;
  }
  if( digitalRead(BUT1) == LOW && digitalRead(BUT2) == LOW )
  {
     digitalWrite(LED1, LOW);
     while(1);
  }
  unsigned long now = millis();
  if(now >= old+timee)
  {
      old = now;
      state = !state;
      digitalWrite(LED1, state);
  }

}
