//MyOneWire

#ifndef MyOneWire_h
#define MyOneWire_h

#include "Arduino.h"

//Arraygroesse fuer Int-, Long- und Float-Zahlen 
#define charInt 7
#define charIntBase 17
#define charLong 12
#define charLongBase 33
#define charFloat 14

class MyOneWire
{
  public:
  MyOneWire(void);
  void init(byte);
  void writeln(void);
  void write(int);
  void write(int, byte);
  void writeln(int);
  void writeln(int, byte);
  void write(unsigned int, byte);
  void writeln(unsigned int, byte);
  void write(unsigned int);
  void writeln(unsigned int);
  void write(long);
  void write(long, byte);
  void writeln(long);
  void writeln(long, byte);
  void write(unsigned long);
  void write(unsigned long, byte);
  void writeln(unsigned long);
  void writeln(unsigned long, byte);
  void write(float);
  void write(float, byte);
  void writeln(float);
  void writeln(float, byte);
  void write(double);
  void writeln(double);
  void write(double, byte);
  void writeln(double, byte);
  void write(char);
  void writeln(char);
  void write(char*, byte);
  void write(const char*, byte);
  void writeln(char*, byte);
  void writeln(const char*, byte);
  void write(char*);
  void write(const char*);
  void writeln(char*);
  void writeln(const char*);
        
  private:
  void send(char);
  void pwm(int);
  void ftoa(float, char*, byte);
  byte PORTBx;
  byte maxDecFloat = 6; //Maximale Anzahl von Nachkommastellen
  int delayBetweenBits;
  int delayBetweenBytes;
  int pwmLow;
  int pwmHigh;
};

#endif
