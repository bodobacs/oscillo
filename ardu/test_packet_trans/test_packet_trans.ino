//Sends packets to pc. Data just bytes 0-4.

#include "commonheader.h" //softlink to the real header!

volatile sserialpacket packets[max_packet_per_msg];

void setup()
{

  for(int i = 0; i < max_packet_per_msg; i++)
  {
    for(int j = 0; j < packet_buffer_size; j++)
    {
      packets[i].h.n_th = i;
      packets[i].buffer[j] = (unsigned char)(i*10);
    }
  }

  Serial.begin(115200); //9600 115200 230400, 345600, 460800
  while(!Serial);
}

int packet_index = 0;

void loop()
{
    Serial.write((byte *)&(packets[packet_index]), packet_size);
    packet_index *= ++packet_index < max_packet_per_msg; // XD

    delay(50); //a small pause very needed, to recieve the correct number of bytes
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
