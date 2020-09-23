//Testprogramm für MyMOneWire-Library
//Code fuer Attiny85
//Author Retian
//Version 1.1



#include "MyOneWire.h"
MyOneWire MOneW;



char text[] = "**** Das ist ein Testprogramm fuer die MyOneWire-Library ****";
float zahl1 = -45.35645;
const float pi = 3.1415926;
long zahl2 = 2147483647;



void setup() {
  MOneW.init(3); //OneWire-Datenleitung auf PB3
}



void loop() {
  MOneW.writeln(); //Zeilenvorschub
  MOneW.writeln(text);

  MOneW.write("Int-Zahl: ");
  MOneW.writeln(125);
  MOneW.write("Int-Zahl im HEX-Format: ");
  MOneW.writeln(125, HEX);

  MOneW.write("Float-Zahl mit 3 Kommastellen: ");
  MOneW.writeln(zahl1, 3);
  MOneW.write("Float-Zahl mit 2 Kommastellen: ");
  MOneW.writeln(2.56897E+4);
  MOneW.write("Die Zahl Pi mit 5 Kommastellen: ");
  MOneW.writeln(pi, 5);

  MOneW.write("Groesstmoegliche Long-Zahl: ");
  MOneW.writeln(zahl2);
  MOneW.write("Groesstmoegliche Long-Zahl im BIN-Format: ");
  MOneW.writeln(2147483647, BIN);
 
  delay(5000);
}
