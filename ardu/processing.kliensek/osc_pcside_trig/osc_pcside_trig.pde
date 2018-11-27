import processing.serial.*;

Serial port;  // Create object from Serial class

byte[] values = null;
float zoom;

void setup() 
{
  size(1010, 500);
  
  // Open the port that the board is connected to and use the same speed (9600 bps)
  port = new Serial(this, Serial.list()[0], 9600);
  printArray(Serial.list());

  values = new byte[1000];
  
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
  draw3();
}

void draw1()
{
  if(!pause)
  {
  //adatok beolvasása soros portról
  for(int j = 0; j < port.available(); j++)
    {
       last++;
       if(last == values.length) last = 0;
       values[last] = byte(port.read());
       println(" ", int(0xff & values[last]));
    }
  }
  background(0);

  stroke(255, 0, 0); //piros
  line(0, height-10, width, height-10);

  stroke(255);//fehér

  int i = last;
  int x = 0;
  int scale = 1;
  do{
    point(++x, height - 10 - (scale * int(0xff & values[i])));
    i++;
    if(i == values.length) i = 0;
  }while(i != last);
}

void draw2()
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
  background(0);

  stroke(255, 0, 0); //piros
  line(0, height-10, width, height-10);

  stroke(255);//fehér

  int scale = 1;

  for(int k = 0; k < width && k < values.length; k++)
  {
    point(k, height - 10 - (scale * int(0xff & values[k])));
  }
}

void draw3()
{
  if(port.available() > 0) //<>//
  {
    port.readBytes(values);

    background(0);
   
    stroke(255, 0, 0); //piros
    line(0, height/2, width, height/2);
  
    
    stroke(255);//fehér
    
    printArray(values);
    
    for(int k = 0; k < values.length; k++)
    {
      point(k, (int(0xff & values[k])));
    }
  }
}