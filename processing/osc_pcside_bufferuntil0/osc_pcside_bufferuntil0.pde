import processing.serial.*;

Serial port;  // Create object from Serial class

byte[] values = null;
float zoom;

void setup() 
{
  size(1250, 600);
  
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
      if(nread > 1000){ saveBytes("numbers.dat", values); }
      break;
    case 's':
      pause = !pause;
      println("pause");
      break;
  }
}

boolean pause = false;
int nread = 0;  
boolean once = true;
void serialEvent(Serial p)
{
  int av;
  if(1 < (av = p.available()))
  {
    nread = p.readBytesUntil(0, values); //amig nullát nem talál
    println("Available: ", av, "bytes read: ", nread );
  }else{
    p.clear();
  }
} 

void draw()
{
    background(0); //<>//
   
    stroke(255, 0, 0); //piros
    line(0, height/2, 500, height/2); //X

    stroke(0, 255, 0); //zold
    line(500, 0, 500, height); //Y
    
    stroke(255);//fehér
   
    int offsetY = height/2;
    for(int k = 0; k < nread; k++)
    {
      point(k, (offsetY - int(0xff & values[k])));
//    point(k, (int(values[k])));
    }
}
