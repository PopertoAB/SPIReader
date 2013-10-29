#ifndef Memoria95040_h
#define Memoria95040_h

#include "Arduino.h"

class Memoria95040{
  public:
    Memoria95040(int selectPin);
    void escribir(int address, byte data);
    byte leer(int address);
    boolean ready();
  private:
    int _selectPin;
};

#endif
