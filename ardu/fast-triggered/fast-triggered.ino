//MEG NEM MUKODIK JOL

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
volatile unsigned int pos_buff = 0;
volatile unsigned int pos_trigger = 0;
volatile unsigned int counter = 0;
volatile unsigned char trigger_level = 0; //AIN1-en mért feszültség

ISR(ANALOG_COMP_vect) //komparator kivaltott egy triggert
{
  bitClear(ADCSRA, ADIE); //komparator trigger kikapcs 
  counter = 0; //új sweep indul
  pos_trigger = pos_buff;  //itt regisztrálom, hogy mikor történt a trigger
}

ISR(ADC_vect)//kész egy AD konverzio
{
  buff[pos_buff] = ADCH;
  counter++;
 
  if(pos_buff + 1 == BUFFER_SIZE)
  {
    pos_buff = 0;
  }else{
    pos_buff++;
  }
  
//   DLOG2("", counter);
//  bitSet(ADCSRA, ADIE); //enable
//  bitClear(ADCSRA, ADIE); //disable
}

void setup(void)
{
//digitális mód kikapcsolása az analog pin-eken
//7:6 reserved=0, ADC5-0 5:0 digital buffer disable, 6-7 no buffer
//bit(ADC5D) | bit(ADC4D) | bit(ADC3D) | bit(ADC2D) | bit(ADC1D) | bit(ADC0D);
//B00111111 = 0x3F
DIDR0 &= 0xC0; DIDR0 |= 0x3F;

//setup Analog Comparator +++++++++++++++++++++++++++++++++++++++++++++++++++++
//disable AIN1 and AIN2 digital buffers
DIDR1 |= 0x03; //AIN1D, AIN0D kikapcsolja a digitalis módot, stabilizálja a bementetet

//ADCSRB=ADC Control and Status Register B: meghatározza, hogy honnan kapja a komparátor a negatív bemenetét
// és mi történjen
//bit 7,5:3 reserved=0 | ACME
//bit 6 ACME: =0 Analog Comparator Multiplexer Enable: az AIN1 helyett a multiplexer adja a negatív bemenetet valamelyik analog pinről, ADC ilyenkor nem működhet
//bit 2:0 ADC Auto Trigger Source (8 modes) -> ADTS[2:0] = ADTS2,ADTS1,ADTS0
//000 Free Running Mode
//001 Analog Comparator és még vagy hat mód
//hogy a triggerelés működjön be kell legyen kapcsolva a ADCSRA(ADEN)=1, ez már megvan
//B0 0 000 001;
ADCSRB &= ~(ACME | ADTS2 | ADTS1 | ADTS0);
ADCSRB |= ADTS0;

//ACSR Analog Comparator Status Register
//7. ACD Analog Comparator Disable bármikor ki lehet vele kapcsolni a comparatort, de változtatás előtt le kell tiltani az Analog Comparator Interrupt-ot is ACSR(ACIE)
//6. ACBG band gap reference voltage kikapcsolása, használja inkább az "AIN0"-t
//5. ACO Analog Comparator Output, csak read, nem értem valami az interrupt fügvényekkel, valami syncronizálás
//4. ACI Analog Comparator Interrupt Flag: hardware változtatja, ACIS1 és ACIS0 registerek által meghatározott interrupt fügvény végrehajtódik, ha ezt a bitet a hardware bekapcsolta
//3. ACIE ha ez a bit be van állítva a Status Register I-bitjével együtt, akkor az interrupt aktiv
//2. ACIC =0
//1. ACIS1 =1
//0. ACIS0 =1
//beállítható, hogy milyen változásra induljon megszakítás, átállítás előtt "ACIE"-val tiltani kell az interruptot
// ACIS1 ACIS0 Mode
// 0     0    Toggle
// 0     1    Reserved
// 1     0    Falling edge
// 1     1    Rising edge
//B0r001111
ACSR &= ~ACO; 
ACSR |= (ACIS1 | ACIS0);

//ADMUX – ADC Multiplexer Selection Register
//7. REFS1
//6. REFS0: referencia feszültséget honnan vegye, 01: AVcc (AREF pin + kapacitás)
//5. ADLAR: 1 balra igazit
//4. reserved=0
//3. MUX3
//2. MUX2
//1. MUX1
//0. MUX0 (melyik pin legyen az ADC bemenete)
  //ADC0 = A0 = 0000
  //ADC1 = A1 = 0001
  //ADC2 = A2 = 0010
  //ADC3 = A3 = 0011
  //ADC4 = A4 = 0100
  //ADC5 = A5 = 0101

ADMUX &= ~(REFS1 | MUX3 | MUX2 | MUX1 | MUX0); //B00010000
ADMUX |= (REFS0 | ADLAR);// | MUX2 | MUX0); //B011x0000 REFS1, REFS0, ADLAR, rserved, MUX3, MUX2, MUX1, MUX0

//ADCSRA – ADC Control and Status Register A 
//7. ADEN: ADC enable (conversion ON/OFF)
//6. ADSC: write: 1 --> ADC start conversion, 0 --> no effect; read: conversion in progress or not
//5. ADATE: ADC auto triggering enable (+ADCSRB:ADTS select trigger)
//4. ADIF: ADC Interrupt Flag read: conversion complete & Data Register updated --> 1, Interrupt Execution --> 0 (if modified can disturb/cancel an interrupt)
//3. ADIE: ADC Interrupt Enable: enabled + SREG(I) bit --> ADC Conversion Complete Interrupt
//2. ADPS2: AD prescaling 10bit --> 50-200kHz, <10bit precision --> >200kHz sampling rate
//1. ADPS1
//0. ADPS0

ADCSRA &= 0x10; //B00010000
ADCSRA |= 0xE8; //B111x1000 ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0 //do not want to change ADIF everything else is 1

Serial.begin(9600);//9600 115200 230400, 345600, 460800
while(!Serial);

DLOG2("DIDR0: ", DIDR0)
DLOG2("DIDR1: ", DIDR1)
DLOG2("ADMUX1: ", ADMUX)
DLOG2("ADCSRB: ", ADCSRB);
DLOG2("ACSR: ", ACSR);
DLOG2("ADCSRA: ", ADCSRA)
}

void loop()
{
  //  while(!Serial.available());
  // put your main code here, to run repeatedly:

  if(counter == BUFFER_SIZE) //a múltból 200 beolvasást megtartok, hogy lássam mi volt a trigger előtt
  {
    Serial.write((char *)buff, BUFFER_SIZE);
//  delay(2000);
    bitSet(ADCSRA, ADIE); //restart interrupts
  }
}
