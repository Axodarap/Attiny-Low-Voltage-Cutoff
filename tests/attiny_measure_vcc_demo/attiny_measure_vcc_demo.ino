void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(3,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t in;
  double vref;
    
  in=MeasureVCC();    
  vref=1024*1.1f/(double)in;

  if(vref >= 3)
  {
    digitalWrite(4,LOW);
  }
  else
  {
    digitalWrite(4,HIGH);
  }
}


uint16_t MeasureVCC(void)
{
    PRR    &=~(1 << PRADC);                                           // power ADC (p. 38)
    ADCSRA  = (1 << ADEN)| (1 << ADPS2)| (1 << ADPS1)| (1 << ADPS0);  // enable ADC, Set prescaler to 128 (p.136)
    ADMUX   = (1 << REFS2) | 0x0c;                                    // set vcc as reference voltage + bangap ref as adc input (p. 134f)
    _delay_ms(1);                                                     // settling time, see p.134 in ATtiny Datasheet

    ADCSRA |= (1 << ADSC);                                            // start conversion (p. 136)
    while (!(ADCSRA & (1 << ADIF)));                                  // wait for conversino to complete, ~100 us (p.136)
    ADCSRA |= (1 << ADIF);                                            // clear ADIF (p. 136)
    return ADC;
}
