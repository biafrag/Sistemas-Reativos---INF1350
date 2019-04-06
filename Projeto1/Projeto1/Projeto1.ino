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
unsigned long tempoBuzzer = 0;
unsigned long ultimaEdicao = 0;
bool alarmeTocando = false;

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
  pinMode(BUZZER,OUTPUT);
  digitalWrite(BUZZER,HIGH);
 
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

void piscaDisplay(Horario horario)
{
  if((millis()/500)%3)
  {
    mostraHorario(horario);
  }
  else
  {
    WriteNumberToSegment(0, 10);
  }
  if(mudaHoras)
  {
    WriteNumberToSegment(2,horario.getMinuto()/10);
    WriteNumberToSegment(3,horario.getMinuto()%10);
  }
  else
  {
     WriteNumberToSegment(0,horario.getHora()/10);
     WriteNumberToSegment(1,horario.getHora()%10);
  }
  
}

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

void checaAlarme()
{
  if(alarmeTocando)
  {
    if(millis() - tempoBuzzer > 500)
    {
        digitalWrite(BUZZER, HIGH); 
        alarmeTocando = false;
    }
  }
}
void editaHorario(Horario& horario)
{
  if(digitalRead(BUT2) == LOW && debounce[1])
  {
    if (mudaHoras)
    {
      horario.avancaHora();
    }
    else
    {
      horario.avancaMinuto(false);
    }
    unsigned long agora = millis();
    timeDebounce[1] = agora;
    debounce[1] = false;
    ultimaEdicao = agora;
  }
}

void voltaAoEstadoInicial()
{
    if(state >= 0)
      digitalWrite(statesLeds[state],OFF);
    state = -1;
}

void loop() {
  // put your main code here, to run repeatedly:
  atualizaHorario();
  for(int i = 0; i < 3 ; i++)
  {
    debounce[i] = bouncing(timeDebounce[i]);
  }
  // Nesse caso o debounce não é um problema
  if(digitalRead(BUT3) == LOW && digitalRead(BUT1) == LOW)
  {
    //Não funcionando ainda
    mostraHorario(horarioAtual);
    voltaAoEstadoInicial();
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
      if(state > 1)
        ultimaEdicao = millis();
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
      if(alarmeAtual == horarioAtual)
      {
        digitalWrite(BUZZER, LOW); 
        alarmeTocando = true;
        tempoBuzzer = millis();
      }
      break;
    case 1:
      mostraHorario(alarmeAtual);
      break;
    case 2:
      piscaDisplay(horarioAtual);
      editaHorario(horarioAtual);
      if(millis() - ultimaEdicao > TIMEOUTEDICAO)
        voltaAoEstadoInicial();
      break;
     case 3:
      piscaDisplay(alarmeAtual);
      editaHorario(alarmeAtual);
      if(millis() - ultimaEdicao > TIMEOUTEDICAO)
        voltaAoEstadoInicial();
      break;
    default:
      mostraHorario(horarioAtual);
    break; 
  }
  checaAlarme();
}
