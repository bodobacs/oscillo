import processing.serial.*;

Serial port;  // Create object from Serial class
byte val;      // Data received from the serial port
byte[] values;
float zoom;

void setup() 
{
  size(1000, 480);
  
  // Open the port that the board is connected to and use the same speed (9600 bps)
  port = new Serial(this, Serial.list()[0], 115200);
  printArray(Serial.list());
  values = new byte[width];
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

int last = 0;
boolean pause = false;
void draw()
{
  if(!pause)
  {
    int j = last < values.length ? last : 0;
    
    //adatok beolvasása soros portról
    for(; 0 < port.available() && j < values.length; j++)
    {
       values[j] = byte(port.read());
       println(" ", int(0xff & values[j]));
    }
  
    last = j;
  }
  background(0); //clear background
  stroke(255, 0, 0); //red x axis
  line(0, height-10, width, height-10);
  stroke(255);//white drawcolor
  int scale = 1; //not used

  for(int k = 0; k < width && k < values.length; k++)
  {
    point(k, height - 10 - (scale * int(0xff & values[k])));
  }
}
