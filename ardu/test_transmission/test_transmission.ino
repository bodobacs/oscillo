//tesztelni kell az átjutó adatcsomagokat

//#define DEBUG

#ifdef DEBUG
#define DLOG(message) Serial.println(message);
#define DLOG2(m1, m2) Serial.print(m1); Serial.println(m2);
#else
#define DLOG(message)
#define DLOG2(m1, m2)
#endif

void setup()
{
  Serial.begin(115200); //9600 115200 230400, 345600, 460800
  while(!Serial);

  DLOG("Setup done.");
}

byte buffer[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
byte buffer2[] = { 21, '\n', 23, 24, 25, 26, 27, 28, 29 };

bool alter = false;

void loop()
{

    Serial.write(0); //null signs start
    if(alter = !alter){
      Serial.write(buffer, 9);
    }else{
      Serial.write(buffer2, 9);
    }

    Serial.flush();
    delay(2000);
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
