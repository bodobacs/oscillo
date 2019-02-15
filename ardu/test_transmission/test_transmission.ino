//NE ÍRD ÁT!!!
//jól működő tesztelő
//255 byteos csomagokat küldözget 3 primszam az elején, 3 primszam a végén

//#define DEBUG

#ifdef DEBUG
#define DLOG(message) Serial.println(message);
#define DLOG2(m1, m2) Serial.print(m1); Serial.println(m2);
#else
#define DLOG(message)
#define DLOG2(m1, m2)
#endif

byte buffer[256];

/*
byte sep_start[3] = { 11, 499, 13 };
byte sep_end[3] = { 491, 23, 487 };
*/

void setup()
{
  buffer[0] = 11;
  buffer[1] = 499;
  buffer[2] = 13;

  buffer[252] = 11;
  buffer[253] = 499;
  buffer[254] = 13;

/*
  buffer[0] = 0;
  buffer[1] = 0;
  buffer[2] = 0;

  buffer[253] = 0;
  buffer[254] = 0;
  buffer[255] = 0;
*/
  //írható terület: buffer[3 - 252] adat 250Byte
  const int idata_start = 3;
  const int idata_end = 252;
  
  for(int i = idata_start; i < idata_end; i++)
  {
    buffer[i] = (byte)i;
  }

  Serial.begin(115200); //9600 115200 230400, 345600, 460800
  while(!Serial);

  DLOG("Setup done.");
}

bool alter = false;

void loop()
{
    buffer[150] = alter ? 000 : 150;
    alter = !alter;

    Serial.write(buffer, 255);

//    Serial.flush();
    delay(10); //a small pause very needed, to recieve the correct number of bytes
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
