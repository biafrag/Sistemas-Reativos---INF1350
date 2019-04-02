
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

  void avancaMinuto(bool propagate = false);

  bool operator==(const Horario& h) const;

private:
  int _hora;
  int _minuto;

};

  

  
  
        
