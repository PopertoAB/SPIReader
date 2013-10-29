#include "Memoria95040.h"
#include "SPI.h"
#include "LiquidCrystal.h"

String command;
char character;
Memoria95040 ext_mem(10);
int boton = 2;
LiquidCrystal lcd(14,15,16,17,18,19);

void setup(){
  Serial.begin(9600);
  Serial.print(">");
  command="";

  lcd.begin(16,2);
  pinMode(boton, INPUT_PULLUP);
}

int estado=0;

void loop(){

  if(digitalRead(boton)==0){
    delay(50);
    String leido="";
    for(int i=164; i<=165; i++){
      byte b=ext_mem.leer(i);
      leido.concat(String((b>>4)&0x0f,HEX));
      leido.concat(String(b&0x0f,HEX));
    }
    leido.toUpperCase();
    lcd.clear();
    lcd.print("X18 Pin:");
    lcd.setCursor(0, 1);
    lcd.print(leido);
  }

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

  /*
  Serial.println("Iniciando escritura");
   
   for(int i=0;i<511;i++){
   ext_mem.escribir(i, (byte)(i%256));
   }
   
   Serial.println("Terminando escritura");
   while(true){};
   */
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
    for(int i=memoriaNum;i<memoriaNum+2;i++){
      byte b=ext_mem.leer(i);
      leido.concat(String((b>>4)&0x0f,HEX));
      leido.concat(String(b&0x0f,HEX));
    }
    leido.toUpperCase();
    Serial.println(leido);
  }
  else{
    Serial.println("No existe comando");
  }
}


