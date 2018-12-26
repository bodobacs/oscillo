import processing.serial.*;

Serial port;  // Create object from Serial class

byte[] values = null;
float zoom;

void setup() 
{
  size(500, 500);
  
  port = new Serial(this, Serial.list()[0], 115200);
  port.bufferUntil(0); //buffering data until 0 found  
//printArray(Serial.list());

  values = new byte[2000];
  
  zoom = 1.0f;
  smooth();
  println("Setup done");
}

void keyPressed() {
  switch (key) {
    case '+':
      println(zoom);
      break;
    case '-':
      break;
    case 's':
      pause = !pause;
      println("pause");
      break;
  }
}

boolean pause = false;
int nread = 0;  

void serialEvent(Serial p)
{
  int av;
  if(0 < (av = port.available()))
  {
    println("Available: ", av );
    nread = port.readBytesUntil(0, values); //amig nullát nem talál
  }
} 

void draw()
{
    background(0); //<>//
   
    stroke(255, 0, 0); //piros
    line(0, height/2, width, height/2);
  
    
    stroke(255);//fehér
    
//    printArray(values);
    
    for(int k = 0; k < nread; k++)
    {
//    point(k, (int(0xff & values[k])));
      point(k, (int(values[k])));
    }
}
