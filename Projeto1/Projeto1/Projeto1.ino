#include "def_pin.h"
#include "horario.h"

#define ON LOW
#define OFF HIGH

/* Constantes de Tempo: */
// Duração de um minuto em milisegundos (para facilitar debug)
#define MIN 1000*2
// Tempo em milisegundos após o qual consideramos que o botão foi pressionado de novo
#define TIMEOUT_DE_BOUNCE 250
// Tempo em milisegundos até que uma edição volte ao estado inicial
#define TIMEOUTEDICAO 10*1000
// Tempo em milisegundos de duração do alarme
#define DURACAO_ALARME 500


/***CÓDIGO PROVIDO***/
/* Define shift register pins used for seven segment display */
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

/*Parte do Display*/
/* Segment byte maps for numbers 0 to 9 */ // Adição de um décimo elemento para poder apagar os LEDs
const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90, 0XFF};
/* Byte maps to select digit 1 to 4 */
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};


/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(byte Segment, byte Value) {
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO,HIGH);
}
/******************/

// Variáveis para controle do bounce
bool debounce[3] = {true,true,true};
float timeDebounce[3] = {500,500,500};

/* Variáveis de estado */

// Modo atual do relógio
// -1: Mostrar horário, alarme desligado
//  0: Mostrar horário, alarme ligado
//  1: Mostrar horário do alarme
//  2: Edição do horário
//  3: Edição do horário de alarme
int modoAtual = -1;
int estadoDosLeds[6] = {LED1,LED2,LED3,LED4};

Horario horarioAtual(0,0);
Horario alarmeAtual(6,0);

// True quando estamos editando horas, False quando estamos editando minutos
bool mudaHoras = true;

// Última vez que os minutos foram alterados
unsigned long referenciaTemporal = 0;
// Última vez que o alarme foi ligado
unsigned long tempoBuzzer = 0;
// Última vez que se fez alguma alteração no horário ou no horário do alarme
unsigned long tempoDaUltimaEdicao = 0;

bool alarmeTocando = false;
// Quantas edições ininterruptas aconteceram
int edicoesSeguidas = 0;
bool modoAvancoRapido = false;

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
      pinMode(estadoDosLeds[i], OUTPUT);
      digitalWrite(estadoDosLeds[i],OFF);
  }

  mostraHorario(horarioAtual);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, OFF);
 
}

// Essa função retorna true caso tenha-se passado tempo suficiente para
// considerar que houve novo pressionamento e false caso contrário
bool bouncing(float timeDebounce)
{
  int fatorRapidez;
  if(modoAvancoRapido)
    fatorRapidez = 2;
  else
    fatorRapidez = 1;
  if(millis() - timeDebounce >= TIMEOUT_DE_BOUNCE / fatorRapidez)
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
  if((millis()/250)%3)
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
  unsigned long agora = millis();
  if((agora - referenciaTemporal) > MIN )
  {
    horarioAtual.avancaMinuto(true);
    referenciaTemporal = agora;
  }
}

void checaAlarme()
{
  if(alarmeTocando)
  {
    if(millis() - tempoBuzzer > DURACAO_ALARME)
    {
        digitalWrite(BUZZER, OFF); 
        alarmeTocando = false;
    }
  }
}

void editaHorario(Horario& horario)
{
  if(digitalRead(BUT2) == ON && debounce[1])
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
    tempoDaUltimaEdicao = agora;
    edicoesSeguidas ++;
    if(edicoesSeguidas > 10)
      modoAvancoRapido = true;
  }
  else if(digitalRead(BUT2) == OFF && !debounce[1])
  {
    edicoesSeguidas = 0;
    modoAvancoRapido = false;
  }
}

void voltaAoEstadoInicial()
{
    if(modoAtual >= 0)
      digitalWrite(estadoDosLeds[modoAtual],OFF);
    modoAtual = -1;
}

void loop() {
  // put your main code here, to run repeatedly:
  atualizaHorario();
  for(int i = 0; i < 3 ; i++)
  {
    debounce[i] = bouncing(timeDebounce[i]);
  }
  // Botão 3 + Botão 1 voltam ao modo inicial
  // Nesse caso o debounce não é um problema
  if(digitalRead(BUT3) == ON && digitalRead(BUT1) == ON)
  {
    mostraHorario(horarioAtual);
    voltaAoEstadoInicial();
    timeDebounce[0] = millis();
    debounce[0] = false;
    timeDebounce[2] = millis();
    debounce[2] = false;
  }
  // Botão 3 altera o modo
  else if(digitalRead(BUT3) == ON && debounce[2] )
  {
    if(modoAtual < 0)
    {
      modoAtual++;
      digitalWrite(estadoDosLeds[modoAtual],ON);
    }
    else if(modoAtual < 3)
    {
      digitalWrite(estadoDosLeds[modoAtual],OFF);
      modoAtual++;    
      digitalWrite(estadoDosLeds[modoAtual],ON);
      if(modoAtual > 1)
        tempoDaUltimaEdicao = millis();
    }
    else
    {
      modoAtual = -1;
      digitalWrite(estadoDosLeds[3],OFF);
    }
    timeDebounce[2] = millis();
    debounce[2] = false;
  }
  // botão 1 alterna entre edição de horas e edição de minutos
  else if(digitalRead(BUT1) == ON && debounce[0] )
  {
    if(modoAtual >1)
    {
      mudaHoras= !mudaHoras;
      timeDebounce[0] = millis();
      debounce[0] = false;
    }
  }
  //Switch para saber em que estado está
  switch(modoAtual)
  {
    case -1:
      mostraHorario(horarioAtual);
      break;
    case 0:
      mostraHorario(horarioAtual);
      if(alarmeAtual == horarioAtual)
      {
        digitalWrite(BUZZER, ON); 
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
      if(millis() - tempoDaUltimaEdicao > TIMEOUTEDICAO)
        voltaAoEstadoInicial();
      break;
     case 3:
      piscaDisplay(alarmeAtual);
      editaHorario(alarmeAtual);
      if(millis() - tempoDaUltimaEdicao > TIMEOUTEDICAO)
        voltaAoEstadoInicial();
      break;
    default:
      mostraHorario(horarioAtual);
    break; 
  }
  checaAlarme();
}
