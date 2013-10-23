#include <SPI.h>

String command;
char character;

void setup(){
  Serial.begin(9600);
  Serial.print(">");
  command="";
  
  SPI.begin();

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE0);
}

void loop(){
  if(Serial.available() > 0){
    character=Serial.read();
    Serial.print(character);
    command.concat(character);
    
    if(character=='\r'){
      procesarComando(command);
      Serial.print(">");
      command="";
    }
    
    character='\0';
  }
}

void procesarComando(String command){
  if(command.startsWith("leer")){
    String memoria=command.substring(
      command.indexOf(",")+1,
      command.length());
    char __memoria[memoria.length()];
    memoria.toCharArray(__memoria,memoria.length());
    int memoriaNum=atoi(__memoria);
    
    String leido;
    for(int i=memoriaNum;i<memoriaNum+4;i++){
      leido.concat((char)read_ext_eeprom(i));
    }
    
    Serial.println(leido);
  }else{
    Serial.println("No existe comando");
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
