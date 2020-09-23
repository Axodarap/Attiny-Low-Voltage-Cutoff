#include <avr/sleep.h>
#include <avr/wdt.h>

#define FET_PIN 4
#define BTN_PIN 3

#define BTN_THRESHOLD 3000

bool current_btn_state = false;
bool previous_btn_state = false;
bool ready_2_call = true;
unsigned long last_press_time  = 0;
unsigned long released_time = 0;


// ---------------------------------------------------- main code ---------------------------------------------------
void setup() 
{
  pinMode(FET_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  setupWatchdog();
  sleep();
}


void loop() 
{
  if(detectLongPress())
  {
    //TODO
    //read current vcc multiples times, average result
    //store value in eeprom
    //confirm with blinking leds
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
  //TODO see above
}

/*
  measures VCC
*/
double measureVCC(void)
{
    PRR    &=~(1 << PRADC);                                           // power ADC (p. 38)
    ADCSRA  = (1 << ADEN)| (1 << ADPS2)| (1 << ADPS1)| (1 << ADPS0);  // enable ADC, Set prescaler to 128 (p.136)
    ADMUX   = (1 << REFS2) | 0x0c;                                    // set vcc as reference voltage + bangap ref as adc input (p. 134f)
    _delay_ms(1);                                                     // settling time, see p.134 in ATtiny Datasheet

    ADCSRA |= (1 << ADSC);                                            // start conversion (p. 136)
    while (!(ADCSRA & (1 << ADIF)));                                  // wait for conversino to complete, ~100 us (p.136)
    ADCSRA |= (1 << ADIF);                                            // clear ADIF (p. 136)
    return 1024*1.1f/(double)ADC;
}

/*
  as the name implies, detects a long push of the button connected to the BTN_PIN
*/
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
  if(measureVCC() <= 3)
  {
    digitalWrite(FET_PIN, LOW);
  }
  else
  {
    digitalWrite(FET_PIN, HIGH);
  }
}

ISR(PCINT0_vect)
{
  //dummy button ISR
}
