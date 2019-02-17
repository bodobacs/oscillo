//packet-es megoldas tesztelése

#include "commonheader.h" //softlink to the real header!

//#define DEBUG

#ifdef DEBUG
#define DLOG(message) Serial.println(message);
#define DLOG2(m1, m2) Serial.print(m1); Serial.println(m2);
#else
#define DLOG(message)
#define DLOG2(m1, m2)
#endif

volatile sserialpacket packets[max_packet_per_msg];

void setup()
{

  for(int i = 0; i < max_packet_per_msg; i++)
  {
    for(int j = 0; j < packet_buffer_size; j++)
    {
      packets[i].buffer[j] = (unsigned char)(i*10);
    }
  }

  Serial.begin(115200); //9600 115200 230400, 345600, 460800
  while(!Serial);

  DLOG("Setup done.");
}

int packet_index = 0;

void loop()
{
  Serial.write((byte *)&(packets[packet_index]), packet_size);

  packet_index *= ++packet_index < max_packet_per_msg; // XD

//  Serial.flush();
    delay(100); //a small pause very needed, to recieve the correct number of bytes
}

//parancskezelés ha kell
void serialEvent()
{
  while (Serial.available())
  {
    char command = (char)Serial.read();
      switch (command)
      {
        case 'D':
        break;

        case 'd':
        break;

        default:
          continue;
      }//switch
  }//while
}
