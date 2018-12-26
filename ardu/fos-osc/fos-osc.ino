#define ANALOG_IN 0

void setup() {
  Serial.begin(9600); 
  //Serial.begin(115200); 
}

void loop() {
  int val = analogRead(ANALOG_IN);                                              
  Serial.write( 0xff );                                                         
  Serial.write( (val >> 2) & 0xff ); //valoszinuleg a 10bites meres ezért csak 2-t kell tolni rajta
  Serial.write( val & 0xff );
}
