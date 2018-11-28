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

const int BUFFER_SIZE = 500;
volatile int forward_size = 250; //read this many after trigger
volatile unsigned char buff[BUFFER_SIZE];
volatile unsigned int buff_pos = 0;
volatile unsigned char full = 0;
volatile unsigned int next_stop = BUFFER_SIZE - 1; //here stop and send data

struct packet
{
  unsigned char buffer[];
};

ISR(ANALOG_COMP_vect)
{
  next_stop = buff_pos + forward_size < BUFFER_SIZE ? buff_pos + forward_size : buff_pos + forward_size - BUFFER_SIZE;
  bitClear(ACSR, ACIE); //disable comparator interrupts
}

ISR(ADC_vect) //a ADC conversion happened
{
  buff[buff_pos] = ADCH; //save voltage value
  buff_pos + 1 == BUFFER_SIZE ? buff_pos = 0 :  buff_pos++;

  if(buff_pos == next_stop)
  {

    full = 1;
    bitClear(ADCSRA, ADIE); //disable ADC interrupts
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

//ADCSRA:ADATE = 1 //enable to select trigger mode
//ADTS2,ADTS1,ADTS0 = 000 Free Running Mode -> Continous conversions
//ADCSRB = ADTS0; //  = 001 Analog Comparator triggers -> ADC_COMP_vect

ADCSRB = 0; //Free Running
DLOG2("ADCSRB: ", ADCSRB);

}

//setup Analog Comparator
void setup_AC(void)
{
ACSR &= ~(bit(ACIE) | bit(ACD)); //comparator interrupt disable

//disable AIN1 and AIN2 digital buffers for analog use or power save
DIDR1 |= bit(AIN1D) | bit(AIN0D);
DLOG2("DIDR1", DIDR1)

//ACSR = Analog Comparator Control and Status Register
//:ACD = 1 -> Analog Comparator Disable
//3:ACIE = 1 -> Analog Comparator Interrupt Enable
//6:ACBG = 1 -> Analog Comparator Bandgap Select (fixed bandgap reference voltage); 0 -> AIN0; selects comparator positive input
//1:0:ACISn:Analog Comparator Interrupt Mode Select
//ACIS1 ACIS0 = 1 1 -> triggered by rising output edge

ACSR &= ~bit(ACBG);
ACSR |= bit(ACIS1) | bit(ACIS0);

//ACSR |= bit(ACIE); //comparator interrupt enable
}

void setup()
{
  Serial.begin(115200); //9600 115200 230400, 345600, 460800
  while(!Serial);
  
  setup_AC();
  DLOG("Setup AC done.");
  
  setup_ADC();
  DLOG("Setup ADC done.");
  
  DLOG("Setup done.");
  pinMode(13, OUTPUT); //blink
}
volatile char blink = HIGH;

void loop()
{
  //visual feedback
  digitalWrite(13, blink = blink == HIGH ? LOW : HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);

  if(full)
  {
    Serial.write((unsigned char *)&(buff[next_stop]), BUFFER_SIZE - next_stop);
    Serial.flush();
    Serial.write((unsigned char *)&(buff[0]), next_stop + 1);    

    full = 0;
      //reenable interrupts
    bitSet(ADCSRA, ADIE);
    bitSet(ACSR, ACIE);
  }
}
