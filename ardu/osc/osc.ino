#define ANALOG_IN A0
#define BUFFER_SIZE 500

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Loop start...");
}



void loop(){
//  while(!Serial.available());
  // put your main code here, to run repeatedly:
  //trigger?
  //mintavételi freq?
  int buff[BUFFER_SIZE];
  for(int i=0; i<BUFFER_SIZE; i++) buff[analogRead(ANALOG_IN)];
  Serial.write((char *)buff, BUFFER_SIZE * sizeof(int));
  delay(100);

/*  int j = 100 * sin(6.28/float(millis() % 1000));
  Serial.write(char(j >> 8));
  Serial.write(char(j & 0x00ff));
*/
 //  Serial.println(100*sin(millis()/1000.0));
 // Serial.println(analogRead(ANALOG_IN));  
  //tárolás meghatározott ideig
  //küldés

}

