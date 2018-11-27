//MUKODIK AD JELET

#define PROBE_PIN A0 //ADC0

//#define DEBUG

#ifdef DEBUG
#define DLOG(message) Serial.println(message);
#define DLOG2(m1, m2) Serial.print(m1); Serial.println(m2);
#else
#define DLOG(message)
#define DLOG2(m1, m2)
#endif

const int BUFFER_SIZE = 1000;
volatile unsigned char buff[BUFFER_SIZE];
volatile unsigned int buff_pos = 0;
volatile unsigned char full = 0;

ISR(ADC_vect)
{
  buff[buff_pos] = ADCH;
  if(buff_pos + 1 == BUFFER_SIZE)
  {
    buff_pos = 0;
    full = 1;
    bitClear(ADCSRA, ADIE); //disable
  }else{
    buff_pos++;
  }

//  bitSet(ADCSRA, ADIE); //enable
//  bitClear(ADCSRA, ADIE); //disable
}

//setup A/D converter
void setup_ADC(void)
{
//digitális mód kikapcsolása az analog pin-eken
//7:6 reserved=0, ADC5-0 5:0 digital buffer disable, 6-7 no buffer
DIDR0 = bit(ADC5D) | bit(ADC4D) | bit(ADC3D) | bit(ADC2D) | bit(ADC1D) | bit(ADC0D);
DLOG2("DIDR0: ", DIDR0)
//ADMUX – ADC Multiplexer Selection Register
//REFS1,REFS0 | ADLAR | reserved=0 | MUX3,MUX2,MUX1,MUX0
//REFS1,REFS0 (referencia feszültséget honnan vegye, ez a VCC) --> 01
//ADLAR(balra igazit)                                          --> 1
//reserved=0                                                   --> 0
//MUX3,MUX2,MUX1,MUX0 (melyik pin legyen az ADC bemenete)      --> PROBE_PIN = ADC0 = A0
ADMUX = bit(REFS0) | bit(ADLAR);
DLOG2("PROBE_PIN: ", PROBE_PIN)
DLOG2("ADMUX: ", ADMUX)
//ADCSRA – ADC Control and Status Register A
//ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
//ADEN: ADC enable (conversion ON/OFF)
//ADSC: write: 1 --> ADC start conversion, 0 --> no effect; read: conversion in progress or not
//ADATE: ADC auto triggering enable (+ADCSRB:ADTS select trigger)
//ADIF: ADC Interrupt Flag read: conversion complete & Data Register updated --> 1, Interrupt Execution --> 0 (if modified can disturb/cancel an interrupt)
//ADIE: ADC Interrupt Enable: enabled + SREG(I) bit --> ADC Conversion Complete Interrupt
//ADPS2: AD prescaling 10bit --> 50-200kHz, <10bit precision --> >200kHz sampling rate
//ADPS1
//ADPS0

ADCSRA |= bit(ADEN) | bit(ADSC) | bit(ADATE) | bit(ADIE);// | bit(ADPS2) | bit(ADPS1) | bit(ADPS0); //do not want to change ADIF everything else is 1
DLOG2("ADCSRA: ", ADCSRA)
//ADCSRB ADC Control and Status Register B
//bit 7,5:3 reserved=0 | ACME
//bit 6 ACME: =0 Analog Comparator Multiplexer Enable (Cannot work together with ADC! Can change comparator negative input, default is AIN1)
//bit 2:0 ADC Auto Trigger Source (8 modes)
//ADTS2,ADTS1,ADTS0 = 000 Free Running Mode

ADCSRB = 0;
DLOG2("ADCSRB: ", ADCSRB);
}

//setup Analog Comparator
void setup_AC(void)
{
//disable AIN1 and AIN2 digital buffers
DIDR1 |= bit(AIN1D) | bit(AIN0D);
DLOG2("DIDR1", DIDR1)
//ACSR register
ACSR &= ~(bit(ACD));
}

void setup()
{
  Serial.begin(9600); //9600 115200 230400, 345600, 460800
  while(!Serial);
  
  setup_AC();
  DLOG("Setup AC done.");
  
  setup_ADC();
  DLOG("Setup ADC done.");
  
  
  DLOG("Setup done.");
}

void loop()
{
  //  while(!Serial.available());
  // put your main code here, to run repeatedly:
  //trigger?
  //mintavételi freq?

  if(full)
  {
    Serial.write((char *)buff, BUFFER_SIZE);
//    delay(2000);
    full = 0;
    bitSet(ADCSRA, ADIE); //restart interrupts
  }
}
