#include <avr/sleep.h>
#include <avr/wdt.h>
#include <EEPROM.h>

#define FET_PIN 4
#define BTN_PIN 3
#define STAT_LED_PIN 2
#define BTN_THRESHOLD 3000

bool curent_btn_state = false;
bool previous_btn_state = false;
bool ready_2_call = true;
unsigned long last_press_time  = 0;
unsigned long released_time = 0;
double voltage_threshold = 0;
double hysterese = 0.1;   

// ---------------------------------------------------- main code ---------------------------------------------------
void setup() 
{
  pinMode(FET_PIN, OUTPUT);
  pinMode(STAT_LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

digitalWrite(STAT_LED_PIN, HIGH);
  EEPROM.get(0, voltage_threshold);  //reads voltage_threshold from EEPROM
  digitalWrite(STAT_LED_PIN, LOW);
  powerMonitor();   //initial power reading, will be repeated periodically in ISR of watchdog timer
  setupWatchdog();
  sleep();
}


void loop() 
{
  if(detectLongPress())
  {
    setThreshold();
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
  cli();                   // clear global interrupt flag in SREG
  wdt_reset();             // reset Watchdog Timer
  MCUSR &=~(1 << WDRF);    // reset watchdog reset flag (p. 44f)
  WDTCR  = (1 << WDCE);    // set watchdog change enable
  WDTCR  = (1 << WDP3);    // set watchdog timer prescale to 4s (p. 46)
  WDTCR |= (1 << WDIE);    // set watchdog timer interrupt
  //configuring Pin-Change-Interrupt
  GIMSK |= (1 << PCIE);    // set pin change interrupt enable
  PCMSK |= (1 << PCINT3);  //(PCINT3)  ==> set Pin PB3 as the interrupt pin
  sei();                   // enable global interrupt flag in SREG
}

/*
  sets attiny into sleep mode
*/
void sleep()
{
  byte adcsra;

  adcsra = ADCSRA;                    //backup adc control and status register
  ADCSRA &= ~(1 << ADEN);             //disable ADC (p. 136)
  MCUCR |= (1 << SM1) & ~(1 << SM0);  //set sleep mode to power down (p. 38)
  MCUCR |= (1 << SE);                 //enable sleep mode (p. 37)
  sleep_cpu();                        //sleep
  MCUCR &= ~(1 << SE);                //disable sleep mode (p. 37)
  ADCSRA = adcsra;                    //reset control and status register to previously saved state
}

/*
  configures the threshold at which the cutoff is triggered
*/
void setThreshold()
{
  double buf = 0;
  short n = 30;
  
  for(int i=0; i<n; i++)
  {
    buf += measureVCC(); 
  }
  voltage_threshold = buf/n;
  EEPROM.put(0, voltage_threshold); 

 for(int j=0; j<5; j++)
 {
  digitalWrite(STAT_LED_PIN, HIGH);
  delay(250);
  digitalWrite(STAT_LED_PIN, LOW);
  delay(250);
 }
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
    curent_btn_state = digitalRead(BTN_PIN);

   if(curent_btn_state)
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
   
    previous_btn_state = curent_btn_state;
    return false;
}

/*
  turns the mosfet off/on accordign to curent voltage level
*/
void powerMonitor()
{
  if(measureVCC() <= voltage_threshold)
  {
    digitalWrite(FET_PIN, LOW);
  }
  else if(measureVCC() >= (voltage_threshold + hysterese))
  {
    digitalWrite(FET_PIN, HIGH);
  }
}

/*
  watchdog interrupt service routine
*/
ISR(WDT_vect)
{
  //Good morning
  powerMonitor();
}

ISR(PCINT0_vect)
{
  //dummy button ISR
}
