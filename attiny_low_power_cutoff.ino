//Sleep Mode mit WDT-Wecken
//Code fuer Attiny
//Author Retian
//Version 1.0


#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED_PIN 4
#define POT_PIN 2
#define BTN_PIN 3

#define BTN_THRESHOLD 3000

bool current_btn_state = false;
bool previous_btn_state = false;
bool ready_2_call = true;
unsigned long last_press_time  = 0;
unsigned long released_time = 0;

void sleep(void);
void setConfig();

// ---------------------------------------------------- main code ---------------------------------------------------
void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(BTN_PIN, INPUT);

  setupWatchdog();
  sleep();
}


void loop() 
{
  if(detectLongPress())
  {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  else if (!digitalRead(BTN_PIN)) //if button is not pressed atm
  {
    sleep();
  }
}

// ---------------------------------------------- additional functions ---------------------------------------------
/*
  configures the watchdog of the attiny
*/
void setupWatchdog()
{
    //Setzen des Registers fuer Watchdog Time-out Interrupt
  cli();
  wdt_reset(); // Reset Watchdog Timer
  MCUSR &= ~(1 << WDRF); //Ruecksetzen des Watchdog System Reset Flag
  WDTCR = (1 << WDCE); //Watchdog Change Enable setzen
  WDTCR = (1 << WDP3); //Watchdog Zyklus = 4 s
  WDTCR |= (1 << WDIE); //Watchdog Timeout Interrupt Enable setzen
  //configuring Pin-Change-Interrupt
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << PCINT3); //(PCINT3)  ==> Pin PB3
  sei();
}


/*
  sets attiny into sleep mode
*/
void sleep()
{
  byte adcsra;

  adcsra = ADCSRA; //ADC Control and Status Register A sichern
  ADCSRA &= ~(1 << ADEN); //ADC ausschalten

  MCUCR |= (1 << SM1) & ~(1 << SM0); //Sleep-Modus = Power Down
  MCUCR |= (1 << SE); //Sleep Enable setzen
  sleep_cpu(); //Schlafe ....
  MCUCR &= ~(1 << SE); //Sleep Disable setzen

  ADCSRA = adcsra; //ADCSRA-Register rueckspeichern
}


/*
  configures the threshold at which the cutoff is triggered
*/
void setThreshold()
{
 
}

bool detectLongPress()
{
    current_btn_state = digitalRead(BTN_PIN);

   if(current_btn_state)
   {
    if(!previous_btn_state)  //button pressed
    {
      //Serial.println("btn pressed");
      last_press_time = millis();
    }
    else if(ready_2_call & (millis() - last_press_time) > BTN_THRESHOLD) //button held long enough
    {
      ready_2_call = false; //prevents multiple calls without button going low in the meantime     
      return true;
    }
   }
   else
   {
    ready_2_call = true;
   }
   
    previous_btn_state = current_btn_state;
    return false;
}

/*
  watchdog interrupt service routine
*/
ISR(WDT_vect)
{
  //Good morning
  int a = 0;
  delay(1000);
  a++;
}

ISR(PCINT0_vect)
{
  
}




//Mehr Infos: https://arduino-projekte.webnode.at/meine-projekte/attiny-im-schlafmodus/wecken-mit-wdt/
