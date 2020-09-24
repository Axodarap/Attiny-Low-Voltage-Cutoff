#include <EEPROM.h>

void setup() 
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  double voltage_threshold = 3.14;
  double result;
  EEPROM.put(0,voltage_threshold);

  Serial.println("Data was written to EEPROM...");
  delay(1000);

  EEPROM.get(0, result);
  Serial.print("Voltage threshold: ");
  Serial.println(result);
  
}

void loop() 
{
  
}
