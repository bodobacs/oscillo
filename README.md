# oscillo
Arduino-pc SDL oscilloscope.

2019.03.03.
Do not bother for now. I frequently edit this project, not sure about success.

Developed and tested on linux 64 bit.
Build with cmake (pc/b.sh)
Dependencies: SDL2, modified SDL2 modules are copied inside
For serial connection it uses unistd, termios, read-write(fd, ... I have not got windows in my mind, but I tend to use standard things.

Latest working outputs
Hardware

  Using girino shield https://www.instructables.com/id/Girino-Fast-Arduino-Oscilloscope/
  But continous mode working with just plugging a signal in A5.
  
Arduino side
  Heavily changed Girino project files.
  ardu/girino.pak.trig
  
PC side
  oscsdla [for example /dev/ttyACM0]
  t -> toggle trigger - continous mode
  PgUP/PgDown -> increase/decrease trigger level
  x -> forces an output, if triggered mode stalled because there is no high enough signal
  ESC (ctrl-C in terminal sometimes saves the situation) exit


Testing outputs

PC:
  test_0to0buffer spits out incoming serial data
  test_0to0ncurses its a table shows packets one after another, if incoming data is correct,
    the first few bytes are the same (packetformat in commonheader.h)
arduino:
  test_comparator just counts interrupts happened spits it out to serial monitor

The rest are steps in development, small trials.

Copyright (c) <2019 Zoltan Fabian>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
