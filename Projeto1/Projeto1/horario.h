
#pragma once

class Horario
{
public:
  Horario();
  Horario(int hora, int minuto);
  ~Horario();

  int getHora();

  int getMinuto();

  void setHorario(int hora, int minuto);

  void avancaHora();

  void avancaMinuto();

private:
  int _hora;
  int _minuto;

};

  

  
  
        
