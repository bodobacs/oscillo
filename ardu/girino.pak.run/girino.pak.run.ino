//continous mode pretty good, but there is a mysterious gap in 128 div mode

//TELJESEN A GIRINO KOPPINTASA KIS VALTOZTATÁSOKKAL
//PROCESSING: Buffer0to0Client
//ADC ANALOG_5 PIN
//COMPARATOR: negativ SIGNAL AIN1 PIN6; pozitiv (THRESOLD/PWM) 7
//PWM FREQVENCIA MÉRHETŐ

#include "commonheader.h" //softlink to the real header!

volatile sserialpacketheader pakhead;

const int BUFFER_SIZE = packet_buffer_size * max_packet_per_msg;
volatile unsigned char buff[BUFFER_SIZE];

volatile int buff_pos = 0;

volatile int adc_counter = -1; //mennyit mérjen a trigger után; -1 triggerelés kikapcsolva
volatile int adc_counter_stor = 3*packet_buffer_size; //változtatható legyen a triggerelés utáni hossz

ISR(ANALOG_COMP_vect)  //COMPARATOR triggered
{
  adc_counter = adc_counter_stor; //yet to read
  bitClear(ACSR, ACIE); //comparator stop
}

ISR(ADC_vect) //a ADC conversion happened
{
  buff[buff_pos] = ADCH;
  buff_pos = (buff_pos + 1) % BUFFER_SIZE;

  if(adc_counter < 0) 
  {//not triggered mode
    buff_pos ? : bitClear(ADCSRA, ADIE); //ha növelés után megint nulla akkor megáll az ADC
  }else{ //triggered mode
    --adc_counter ? : bitClear(ADCSRA, ADIE);
  }
}

unsigned char glob_ADC_div = 7; // [0-7]
void set_ADC_div(int d)
{
// These bits determine the division factor between the system clock
// frequency and the input clock to the ADC.
//  ADPS2 ADPS1 ADPS0 Division Factor
// 0: 0 0 0 2
// 1: 0 0 1 2
// 2: 0 1 0 4
// 3: 0 1 1 8
// 4: 1 0 0 16
// 5: 1 0 1 32
// 6: 1 1 0 64
// 7: 1 1 1 128

  if(0 <= d && d < 8)
  {
    ADCSRA &= (255-7);//11111000
    ADCSRA |= d;
    
  /*  
    bitSet(ADCSRA,ADPS2);
    bitSet(ADCSRA,ADPS1);
    bitSet(ADCSRA,ADPS0); //--> 128
  */
  }
 
}

//setup A/D converter
void setup_ADC(void)
{
bitClear(ADCSRA, ADEN); //ADC Enable
bitClear(ADCSRA, ADSC); //ADC Start Conversion
bitClear(ADCSRA, ADIE); //no ADC Interrupt Enable

//digitális mód kikapcsolása az analog pin-eken
//ADC5-0 5:0 digital buffer disable
bitSet(DIDR0, ADC5D);
bitSet(DIDR0, ADC4D);
bitSet(DIDR0, ADC3D);
bitSet(DIDR0, ADC2D);
bitSet(DIDR0, ADC1D);
bitSet(DIDR0, ADC0D);

//DLOG2("DIDR0: ", DIDR0)
//ADMUX – ADC Multiplexer Selection Register

//reference for the ADC
//  REFS1 REFS0 Voltage reference
//  0 0 AREF, Internal Vref turned off
//* 0 1 AVCC with external capacitor at AREF pin
//  1 0 Reserved
//  1 1 Internal 1.1V Voltage Reference with external
//      capacitor at AREF pin
bitClear(ADMUX, REFS1); //??
bitSet(ADMUX, REFS0);

//ADC Left Adjust Result
bitSet(ADMUX, ADLAR);

//Input Channel Selection
//MUX[3:0] Single Ended Input
//0000 ADC0
//0001 ADC1
//0010 ADC2
//0011 ADC3
//0100 ADC4
//0101 ADC5

bitClear(ADMUX, MUX3);
bitSet(ADMUX, MUX2);
bitClear(ADMUX, MUX1);
bitSet(ADMUX, MUX0); //-> ADC5

bitSet(ADCSRA, ADATE); //ADC Auto Trigger Enable

set_ADC_div(5); //ADC clock division

//  ADTS2 ADTS1 ADTS0 Trigger source
//  0 0 0 Free Running mode
//  0 0 1 Analog Comparator
//  0 1 0 External Interrupt Request 0
//  0 1 1 Timer/Counter0 Compare Match A
//  1 0 0 Timer/Counter0 Overflow
//  1 0 1 Timer/Counter1 Compare Match B
//  1 1 0 Timer/Counter1 Overflow
//  1 1 1 Timer/Counter1 Capture Event
bitClear(ADCSRB,ADTS2);
bitClear(ADCSRB,ADTS1);
bitClear(ADCSRB,ADTS0); //--> Free Running Mode

bitSet(ADCSRA, ADEN); //ADC Enable
bitSet(ADCSRA, ADSC); //ADC Start Conversion
bitSet(ADCSRA, ADIE); //ADC Interrupt Enable
}

//setup Analog Comparator
void setup_comparator(void)
{
//ACSR = Analog Comparator Control and Status Register
bitClear(ACSR, ACBG); //Analog Comparator Bandgap Select
bitClear(ACSR, ACD);  //Analog Comparator Disable
bitClear(ACSR, ACIE); //Analog Comparator Interrupt Enable
bitClear(ACSR, ACIC);

bitClear(ADCSRB, ACME); // no Analog Comparator Multiplexer Enable --> AIN1 is applied to the negative input of the Analog Comparator

//disable AIN1 and AIN2 digital buffers for analog use or power save
bitSet(DIDR1, AIN1D);
bitSet(DIDR1, AIN0D);

//Analog Comparator Interrupt Mode Select
bitSet(ACSR, ACIS1);
bitSet(ACSR, ACIS0); //-->Rising edge

bitSet(ACSR, ACIE); //comparator interrupt enable
}

void initPWM(void)
{
  //---------------------------------------------------------------------
  // TCCR2A settings
  //---------------------------------------------------------------------
  // These bits control the Output Compare pin (OC2A) behavior. If one or
  // both of the COM2A1:0 bits are set, the OC2A output overrides the
  // normal port functionality of the I/O pin it is connected to.
  // However, note that the Data Direction Register (DDR) bit
  // corresponding to the OC2A pin must be set in order to enable the
  // output driver.
  // When OC2A is connected to the pin, the function of the COM2A1:0 bits
  // depends on the WGM22:0 bit setting.
  //
  // Fast PWM Mode
  //  COM2A1  COM2A0
  //  0 0 Normal port operation, OC2A disconnected.
  //  0 1 WGM22 = 0: Normal Port Operation, OC0A Disconnected.
  //      WGM22 = 1: Toggle OC2A on Compare Match.
  //  1 0 Clear OC2A on Compare Match, set OC2A at BOTTOM
  //  1 1 Clear OC2A on Compare Match, clear OC2A at BOTTOM
  bitClear(TCCR2A,COM2A1);
  bitClear(TCCR2A,COM2A0);
  bitSet(TCCR2A,COM2B1);
  bitClear(TCCR2A,COM2B0);

  // Combined with the WGM22 bit found in the TCCR2B Register, these bits
  // control the counting sequence of the counter, the source for maximum
  // (TOP) counter value, and what type of waveform generation to be used
  // Modes of operation supported by the Timer/Counter unit are:
  //  - Normal mode (counter),
  //  - Clear Timer on Compare Match (CTC) mode,
  //  - two types of Pulse Width Modulation (PWM) modes.
  //
  //  Mode  WGM22 WGM21 WGM20 Operation TOP
  //  0 0 0 0 Normal    0xFF
  //  1 0 0 1 PWM   0xFF
  //  2 0 1 0 CTC   OCRA
  //  3 0 1 1 Fast PWM  0xFF
  //  4 1 0 0 Reserved  -
  //  5 1 0 1 PWM   OCRA
  //  6 1 1 0 Reserved  -
  //  7 1 1 1 Fast PWM  OCRA
  bitClear(TCCR2B,WGM22);
  bitSet(TCCR2A,WGM21);
  bitSet(TCCR2A,WGM20);

  //---------------------------------------------------------------------
  // TCCR2B settings
  //---------------------------------------------------------------------
  // The FOC2A bit is only active when the WGM bits specify a non-PWM
  // mode.
  // However, for ensuring compatibility with future devices, this bit
  // must be set to zero when TCCR2B is written when operating in PWM
  // mode. When writing a logical one to the FOC2A bit, an immediate
  // Compare Match is forced on the Waveform Generation unit. The OC2A
  // output is changed according to its COM2A1:0 bits setting. Note that
  // the FOC2A bit is implemented as a strobe. Therefore it is the value
  // present in the COM2A1:0 bits that determines the effect of the
  // forced compare.
  // A FOC2A strobe will not generate any interrupt, nor will it clear
  // the timer in CTC mode using OCR2A as TOP.
  // The FOC2A bit is always read as zero.
  bitClear(TCCR2B,FOC2A);
  bitClear(TCCR2B,FOC2B);

  // The three Clock Select bits select the clock source to be used by
  // the Timer/Counter.
  //  CS22  CS21  CS20  Prescaler
  //  0 0 0 No clock source (Timer/Counter stopped).
  //  0 0 1 No prescaling
  //  0 1 0 8
  //  0 1 1 32
  //  1 0 0 64
  //  1 0 1 128
  //  1 1 0 256
  //  1 1 1 1024
  bitClear(TCCR2B,CS22);
  bitClear(TCCR2B,CS21);
  bitSet(TCCR2B,CS20); //--> No prescaling

  pinMode( 13, OUTPUT );
  pinMode( 3, OUTPUT ); //thresholdPin

  analogWrite( 3, 100 );
}

void setup()
{
  buff_pos = 0;
  
//  initPWM();
//  setup_comparator();
  setup_ADC();

  Serial.begin(115200); //9600 115200 230400, 345600, 460800
  while(!Serial);

}

volatile int packet_index = 0;

void loop()
{
  if(adc_counter < 0) //continous mode
  {
    if(buff_pos == 0) //returned to the start time to send data
    {
      pakhead.n_th = packet_index;
      Serial.write((byte *)&pakhead, sizeof(sserialpacketheader));

      int packet_start = packet_index * packet_buffer_size;
      Serial.write((byte *)&(buff[packet_start]), packet_buffer_size);

      delay(20); //a small pause very needed, to recieve the correct number of bytes
      
      if(++packet_index == max_packet_per_msg)
      {
        packet_index = 0;
  
        //reenable interrupts
        bitSet(ADCSRA, ADIE); //enable ADC interrupts vagy ? bitSet(ADCSRA, ADEN); //ON ADC
      }
    }//continous mode

  }else{//triggered mode

  /*
        pakhead.n_th = packet_index;
        Serial.write((byte *)&pakhead, sizeof(sserialpacketheader));
  
        int packet_start = buff_pos + packet_index * packet_buffer_size;
        if(packet_start + packet_buffer_size < BUFFER_SIZE)
        {
          Serial.write((byte *)&(buff[packet_start]), packet_buffer_size);
        }else{
          Serial.write((byte *)&buff[packet_start], BUFFER_SIZE - packet_start);
          Serial.write((byte *)&buff[0], packet_start + packet_buffer_size - BUFFER_SIZE);
        }
  
        delay(20); //a small pause very needed, to recieve the correct number of bytes
        
        if(++packet_index == max_packet_per_msg)
        {
          packet_index = 0;
    
  //        delay(50);
    
          //reenable interrupts
          bitSet(ADCSRA, ADIE); //enable ADC interrupts vagy ? bitSet(ADCSRA, ADEN); //ON ADC
  
  */    
  }
  
}//loop

//parancskezelés ha kell
void serialEvent()
{
  while (Serial.available())
  {
    char command = (char)Serial.read();
      switch (command)
      {
        case 'D':
          set_ADC_div(glob_ADC_div < 7 ? glob_ADC_div++ : 255);
        break;

        case 'd':
          set_ADC_div(glob_ADC_div > 4 ? glob_ADC_div-- : 255);
        break;

        default:
          continue;
      }//switch
  }//while
}
