#include <SPI.h>

byte nombre[]={
  80, 111, 112, 101, 114, 115};

void setup(){
  Serial.begin(115200);

  SPI.begin();

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE0);
}

char mensaje[]={
  'H', 'o', 'l', 'a', ' ', 'm', 'u', 'n', 'd', 'o', '!'};

int estado = 0;
byte leido[512];
int i;
void loop(){
  if(estado==0){
    estado=1;
    for(i=0; i<11; i++){
      write_ext_eeprom(i, mensaje[i]);
    }
    for(i=0; i<512; i++){
      leido[i]=read_ext_eeprom(i);
      Serial.print((char)leido[i]);
      Serial.print(" ");
      if(i%20==0&&i!=0){
        Serial.println("");
      }
    }
  }
}

boolean ext_eeprom_ready(){
  digitalWrite(10, LOW);
  SPI.transfer(0x05);
  byte input = SPI.transfer(0xFF);
  digitalWrite(10, HIGH);

  return ((input & 0x01) == 0);
}

void write_ext_eeprom(int address, byte data){
  while(!ext_eeprom_ready());

  if(address>511 || address<0){
    Serial.println("No existe la direccion dada");
  }
  else{
    digitalWrite(10, LOW);
    SPI.transfer(0x06);
    digitalWrite(10, HIGH);

    digitalWrite(10, LOW);
    if(address>255){
      SPI.transfer((byte)(0x0A));
    }
    else{
      SPI.transfer((byte)(0x02));
    }
    SPI.transfer((byte)(address & 0xFF));
    SPI.transfer(data);
    digitalWrite(10, HIGH);

    delay(5);
  }
}

byte read_ext_eeprom(int address){
  while(!ext_eeprom_ready);

  if(address>511 || address<0){
    Serial.println("No existe la direccion dada");

    return NULL;
  }
  else{
    digitalWrite(10, LOW);
    if(address>255){
      SPI.transfer((byte)(0x0B));
    }
    else{
      SPI.transfer((byte)(0x03));
    }
    SPI.transfer((byte)(address & 0xFF));
    byte leido = SPI.transfer(0xFF);
    digitalWrite(10, HIGH);

    return leido;
  }
}

