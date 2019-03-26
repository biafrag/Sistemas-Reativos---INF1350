#include "horario.h"

Horario::Horario()
{
  
}
Horario::Horario(int hora, int minuto):_hora(hora),_minuto(minuto)
{
  
}
Horario::~Horario()
{
  
}

int  Horario::getHora()
{
  return _hora;
}

int  Horario::getMinuto()
{
  return _minuto;
}

void  Horario::setHorario(int hora, int minuto)
{
  _hora = hora;
  _minuto = minuto;
}

void  Horario::avancaHora()
{
  _hora = (_hora+1)%24;
}

void  Horario::avancaMinuto(int delta)
{
  _minuto = (_minuto + delta)%60;
}

