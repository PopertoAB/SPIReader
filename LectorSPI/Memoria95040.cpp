#include "Arduino.h"
#include "Memoria95040.h"
#include "SPI.h"

Memoria95040::Memoria95040(int pin){
  pinMode(pin, OUTPUT);
  _selectPin=pin;

  SPI.begin();

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE0);
}

void Memoria95040::escribir(int address, byte data){
  while(!Memoria95040::ready());

  if(address>511 || address<0){
    Serial.println("No existe la direccion dada");
  }
  else{
    //Select the chip
    digitalWrite(_selectPin, LOW);

    //Do things
    SPI.transfer(0x06);

    //Unselect the chip
    digitalWrite(_selectPin, HIGH);

    //Select the chip
    digitalWrite(_selectPin, LOW);
    
    //Do things
    if(address>255){
      SPI.transfer((byte)(0x0A));
    }
    else{
      SPI.transfer((byte)(0x02));
    }
    SPI.transfer((byte)(address & 0xFF));
    SPI.transfer(data);

    //Unselect the chip
    digitalWrite(_selectPin, HIGH);

    delay(5);
  }
}

byte Memoria95040::leer(int address){
  while(!Memoria95040::ready());

  if(address>511 || address<0){
    Serial.println("No existe la direccion dada");

    return NULL;
  }
  else{
    //Select the chip
    digitalWrite(_selectPin, LOW);
    
    //Do things
    if(address>255){
      SPI.transfer((byte)(0x0B));
    }
    else{
      SPI.transfer((byte)(0x03));
    }
    SPI.transfer((byte)(address & 0xFF));
    byte leido = SPI.transfer(0xFF);
    
    //Unselect the chip
    digitalWrite(_selectPin, HIGH);

    return leido;
  }
}

boolean Memoria95040::ready(){
  //Select the chip
  digitalWrite(_selectPin, LOW);

  //Do things
  SPI.transfer(0x05);
  byte input = SPI.transfer(0xFF);

  //Unselect the chip
  digitalWrite(_selectPin, HIGH);

  return ((input & 0x01) == 0);
}
