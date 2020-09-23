//MyOneWire.cpp
//Code fuer Attiny85
//Author Retian
//Version 1.3

/*
  Uebertragung von Daten von einem Attiny85 zu einem Arduino ueber nur eine Datenleitung.
  (Wegen des benoetigten Speicherplatzes ist die Library für Attiny45 nur bedingt verwendbar)

  MyOneWire Name;

  Beispiel siehe unter:
  http://arduino-projekte.webnode.at/meine-libraries/attiny-onewire/

  Funktionen siehe unter:
  http://arduino-projekte.webnode.at/meine-libraries/attiny-onewire/funktionen/

*/

//*************************************************************************
//*************************************************************************

#include "Arduino.h"
#include "MyOneWire.h"

MyOneWire::MyOneWire()
{
}

//*************************************************************************
//Initialisierung der Lib

void MyOneWire::init(byte pin)
{
  byte _pin = pin;

  PORTBx = _pin;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);

  byte clockPrescaler = (CLKPR & B1111);

  //Attiny mit 8 MHz Taktfrequenz
  if (clockPrescaler == 0)
  {
    delayBetweenBits = 100; //us
    delayBetweenBytes = 400; //us
    pwmLow = 100; //us
    pwmHigh = 200; //us
  }

  //Attiny mit 1 MHz Taktfrequenz
  else if (clockPrescaler == 3)
  {
    delayBetweenBits = 200; //us
    delayBetweenBytes = 1000; //us
    pwmLow = 400; //us
    pwmHigh = 800; //us
  }
}

//*************************************************************************
//Neue Zeile

void MyOneWire::writeln(void)
{
  send('\n');
}

//*************************************************************************
//Schreibe Interger-Zahl

void MyOneWire::write(int val, byte base)
{
  int _val = val;
  char valChar[charIntBase] = "\0";
  byte _base = base;

  itoa(_val, valChar, _base);

  byte i = 0;
  while (valChar[i] != '\0')
  {
    send(valChar[i]);
    i++;
  }
}

void MyOneWire::writeln(int val, byte base)
{
  write(val, base);
  send('\n');
}

void MyOneWire::write(int val)
{
  write(val, 10);
}

void MyOneWire::writeln(int val)
{
  write(val, 10);
  send('\n');
}

void MyOneWire::write(unsigned int val, byte base)
{
  write((long)val, base);
}

void MyOneWire::writeln(unsigned int val, byte base)
{
  write((long)val, base);
  send('\n');
}

void MyOneWire::write(unsigned int val)
{
  write((long)val, 10);
}

void MyOneWire::writeln(unsigned int val)
{
  write((long)val, 10);
  send('\n');
}

//*************************************************************************
//Schreibe Long-Zahl

void MyOneWire::write(long val, byte base)
{
  long _val = val;
  char valChar[charLongBase] = "\0";
  byte _base = base;

  ltoa(_val, valChar, _base);

  byte i = 0;
  while (valChar[i] != '\0')
  {
    send(valChar[i]);
    i++;
  }
}

void MyOneWire::writeln(long val, byte base)
{
  write(val, base);
  send('\n');
}

void MyOneWire::write(long val)
{
  write(val, 10);
}

void MyOneWire::writeln(long val)
{
  write(val, 10);
  send('\n');
}

void MyOneWire::write(unsigned long val, byte base)
{
  write((long)val, base);
}

void MyOneWire::writeln(unsigned long val, byte base)
{
  write((long)val, base);
  send('\n');
}

void MyOneWire::write(unsigned long val)
{
  write((long)val, 10);
}

void MyOneWire::writeln(unsigned long val)
{
  write((long)val, 10);
  send('\n');
}

//*************************************************************************
//Schreibe Float-Zahl

void MyOneWire::write(float val, byte dec)
{
  float _val = val;
  byte _dec = min(dec, maxDecFloat); //Begrenzung Kommastellen
  char valChar[charFloat] = "\0";

  ftoa(_val, valChar, _dec);

  byte i = 0;
  while (valChar[i] != '\0')
  {
    send(valChar[i]);
    i++;
  }
}

void MyOneWire::writeln(float val, byte dec)
{
  write(val, dec);
  send('\n');
}

void MyOneWire::write(float val)
{
  write(val, 2);
}

void MyOneWire::writeln(float val)
{
  write(val, 2);
  send('\n');
}

void MyOneWire::write(double val)
{
  write((float)val, 2);
}

void MyOneWire::writeln(double val)
{
  write((float)val, 2);
  send('\n');
}

void MyOneWire::write(double val, byte dec)
{
  write((float)val, dec);
}

void MyOneWire::writeln(double val, byte dec)
{
  write((float)val, dec);
  send('\n');
}

//*************************************************************************
//Schreibe Character

void MyOneWire::write(char c)
{
  char _c = c;

  send(_c);
}

void MyOneWire::writeln(char c)
{
  char _c = c;

  send(_c);
  send('\n');
}

//*************************************************************************
//Schreibe Zeichenkette

void MyOneWire::write(char* str)
{
  char* _str = str;
  byte i = 0;

  while (_str[i] != '\0')
  {
    send(_str[i]);
    i++;
  }
}

void MyOneWire::writeln(char* str)
{
  write(str);
  send('\n');
}

void MyOneWire::write(char* str, byte len)
{
  char* _str = str;
  byte _len = len;

  for (byte i = 0; i < _len; i++) send(_str[i]);
}

void MyOneWire::writeln(char* str, byte len)
{
  write(str, len);
  send('\n');
}

void MyOneWire::write(const char* str)
{
  const char* _str = str;
  byte i = 0;

  while (_str[i] != '\0')
  {
    send(_str[i]);
    i++;
  }
}

void MyOneWire::writeln(const char* str)
{
  write(str);
  send('\n');
}

void MyOneWire::write(const char* str, byte len)
{
  const char* _str = str;
  byte _len = len;

  for (byte i = 0; i < _len; i++) send(_str[i]);
}

void MyOneWire::writeln(const char* str, byte len)
{
  write(str, len);
  send('\n');
}

//*************************************************************************
//Uebertrage Zeichen
//(Interne Verwendung)

void MyOneWire::send(char val)
{
  bool datenBit;
  bool parityBit;
  byte n = 0;

  //Ermittle Paritybit
  for (byte i = 0; i < 8; i++)
  {
    if (bitRead(val, i) == 1) n++;
  }
  if (n % 2 == 0) parityBit = 0;
  else parityBit = 1;

  //Sende LOW zum Start
  PORTB &= ~(1 << PORTBx);
  delayMicroseconds(delayBetweenBits);

  //Starte Uebertragung
  for (int8_t bitCount = 9; bitCount > 0; bitCount--)
  {
    if (bitCount > 1)
    {
      datenBit = bitRead(val, bitCount - 2);
      if (datenBit == 0) pwm(pwmLow);
      else if (datenBit == 1) pwm(pwmHigh);
    }
    //Sende Paritybit
    else if (bitCount == 1)
    {
      if (parityBit == 0) pwm(pwmLow);
      else if (parityBit == 1) pwm(pwmHigh);
    }
    delayMicroseconds(delayBetweenBits);
  }
  PORTB |= (1 << PORTBx);
  delayMicroseconds(delayBetweenBytes);
}

//*************************************************************************
//Impulsausgabe
//(Interne Verwendung)

void MyOneWire::pwm(int pwmTime)
{
  PORTB |= (1 << PORTBx);
  delayMicroseconds(pwmTime);
  PORTB &= ~(1 << PORTBx);
}

//*************************************************************************
//Float in String umwandeln
//(Interne Verwendung)

void MyOneWire::ftoa(float floatVal, char* floatStr, byte dec)
{
  long vk = 0;
  long nk = 0;
  float nkf;
  char vkStr[10] = "\0";
  char nkStr[7] = "\0";
  float f;
  float add;
  
  //Additionswert und Faktor fuer Rundung festlegen
  //dec = 0: add = 0.5         f = 1
  //dec = 1: add = 0.05        f = 10
  //dec = 2: add = 0.005       f = 100
  //dec = 3: add = 0.0005      f = 1000
  //dec = 4: add = 0.00005     f = 10000
  //dec = 5: add = 0.000005    f = 100000
  //dec = 6: add = 0.0000005   f = 1000000
  f = 1;
  if (dec > 0)
  {
    for(byte i = 0; i < dec; i++) f *= 10;
  }
  add = 0.5 / f;

  if (floatVal >= 0) floatVal += add;
  else floatVal -= add;

  //Vorkommastellen in String wandeln
  vk = (long)floatVal;
  ltoa(vk, vkStr, 10);

  //Nachkommastellen ermitteln
  nkf = abs(floatVal - vk);

  //Anzahl der "Vornullen" der Nachkommastellen ermitteln
  byte z = 0; //Zaehler fuer Anzahl der "Vornullen"
  while (nkf < 0.1)
  {
    nkf *= 10;
    f /= 10;
    if (z < dec) z++;
    else break;
  }

  //Nachkommastellen (ohne Vornullen) in String umwandeln
  nk = (long)(nkf * f);
  if (nk > 0) ltoa(nk, nkStr, 10);

  //Ergebnis-String zusammenstellen
  //
  //Vorkommastellen in Ergebnis-String kopieren
  strcpy(floatStr, vkStr);
  //Dezimalpunkt und Nachkommastellen an Ergebnis-String anhaengen
  if (dec > 0)
  {
    //Dezimalpunkt an Ergebnis-String anhaengen
    strcat(floatStr, ".");
    //Vornullen an Ergebnis-String anhaengen
    if (z > 0)
    {
      for (byte i = 0; i < z; i++) strcat(floatStr, "0");
    }
    //Nachkommastellen (ohne Vornullen) an Ergebnis-String anhaengen
    strcat(floatStr, nkStr);
  }
  //Begrenzer an Ergebnis-String anhaengen
  strcat(floatStr, "\0");
}