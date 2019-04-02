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

void  Horario::avancaMinuto(bool propagate)
{
  if(_minuto == 59 && propagate)
     avancaHora();
  _minuto = (_minuto + 1)%60;
}

bool Horario::operator==(const Horario& h) const
{
  if(_hora == h.getHora() && _minuto == h.getMinuto())
  {
    return true;
  }
  return false;
}

