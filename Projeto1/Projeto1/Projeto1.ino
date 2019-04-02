#include "def_pin.h"
#include "horario.h"

/* Define shift register pins used for seven segment display */
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

/*Parte do Display*/
/* Segment byte maps for numbers 0 to 9 */
const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90, 0XFF};
/* Byte maps to select digit 1 to 4 */
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};
/******************/

bool debounce[3] = {true,true,true};
float timeDebounce[3] = {600,600,600};
int state = -1;
int statesLeds[6] = {LED1,LED2,LED3,LED4};
Horario horarioAtual(0,0);
Horario alarmeAtual(12,36);
bool mudaHoras = true;
unsigned long tempo = 0;

void setup() {

  /* Set DIO pins to outputs */
  pinMode(LATCH_DIO,OUTPUT);
  pinMode(CLK_DIO,OUTPUT);
  pinMode(DATA_DIO,OUTPUT);
  
  // put your setup code here, to run once:
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);

  for(int i = 0; i < 4; i++)
  {
      pinMode(statesLeds[i], OUTPUT);
      digitalWrite(statesLeds[i],OFF);
  }

  mostraHorario(horarioAtual);
  Serial.begin(9600);
 
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(byte Segment, byte Value) {
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO,HIGH);
}


bool bouncing(float timeDebounce)
{
  if(millis() - timeDebounce >= 250)
  {
    return true;
  }
  return false;
}

void mostraHorario(Horario horario)
{
  WriteNumberToSegment(0,horario.getHora()/10);
  WriteNumberToSegment(1,horario.getHora()%10);
  WriteNumberToSegment(2,horario.getMinuto()/10);
  WriteNumberToSegment(3,horario.getMinuto()%10);
}

void piscaDisplay()
{
  if(mudaHoras)
  {
    
  }
  else
  {
    
  }
  
}

#define SEG 1000*2 
void atualizaHorario()
{
  unsigned long now = millis();
  //TODO: Mudar para 60 segundos ao inves de 10
  if((now - tempo) > SEG )
  {
    horarioAtual.avancaMinuto(true);
    tempo = now;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  atualizaHorario();
  for(int i = 0; i < 3 ; i++)
  {
    debounce[i] = bouncing(timeDebounce[i]);
  }
  if(digitalRead(BUT3) == LOW && debounce[2] && digitalRead(BUT1) == LOW && debounce[0] )
  {
    state = -1;
    timeDebounce[0] = millis();
    debounce[0] = false;
    timeDebounce[2] = millis();
    debounce[2] = false;
  }
  else if(digitalRead(BUT3) == LOW && debounce[2] )
  {
    if(state < 0)
    {
      state++;
      digitalWrite(statesLeds[state],ON);
    }
    else if(state < 3)
    {
      digitalWrite(statesLeds[state],OFF);
      state++;    
      digitalWrite(statesLeds[state],ON);
    }
    else
    {
      state = -1;
      digitalWrite(statesLeds[3],OFF);
    }
    timeDebounce[2] = millis();
    debounce[2] = false;
  }
  else if(digitalRead(BUT1) == LOW && debounce[0] )
  {
    mudaHoras= !mudaHoras;
    timeDebounce[0] = millis();
    debounce[0] = false;
  }
  //Switch para saber em que estado está
  switch(state)
  {
    case -1:
      mostraHorario(horarioAtual);
      break;
    case 0:
      mostraHorario(horarioAtual);
//      if(alarmeAtual == horarioAtual)
//      {
//        Serial.println("Alarmeeee");
//      }
      break;
    case 1:
      mostraHorario(alarmeAtual);
      break;
    case 2:
      if((millis()/500)%3)
        mostraHorario(horarioAtual);
      else
        for (int i = 0;i<2;i++)
        {
          WriteNumberToSegment(i, 10);
        }
      if(digitalRead(BUT2) == LOW && debounce[1])
      {
        if (mudaHoras)
        {
          horarioAtual.avancaHora();
        }
        else
        {
          horarioAtual.avancaMinuto();
        }
        piscaDisplay();
        timeDebounce[1] = millis();
        debounce[1] = false;
      }
      break;
     case 3:
      mostraHorario(alarmeAtual);
      if(digitalRead(BUT2) == LOW && debounce[1])
      {
        if (mudaHoras)
        {
          alarmeAtual.avancaHora();
        }
        else
        {
          alarmeAtual.avancaMinuto();
        }
        piscaDisplay();
        timeDebounce[1] = millis();
        debounce[1] = false;
      }
      break;
    default:
      mostraHorario(horarioAtual);
    break;
    
    
  }
}
