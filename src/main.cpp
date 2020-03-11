#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Encoder.h>
#include <OneButton.h>
#include <Display.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

Display display_z(/* clock=*/ 12, /* data=*/ 11); 
Display display_y(/* clock=*/ 10, /* data=*/ 9);
Display display_x(/* clock=*/ 7, /* data=*/ 8);

Encoder enc(35, 36);
OneButton switchAxis(1, true);

// End of constructor list


signed int x;	// start value
signed int y;	// start value
signed int z;	// start value

signed int *activeAxis = &x;
Display *activeDisplay = &display_x;
int activeAxisIndex = 0;

void setActiveAxisValue(signed int value) {
  *activeAxis = value;
  (*activeDisplay).setValue(*activeAxis);
}

void myDoubleClickFunction() {
  setActiveAxisValue(*activeAxis + 1);
}

void myLongPress() {
  setActiveAxisValue(0);
  enc.write(0);
}

void myClickFunction() {
  activeAxisIndex++;
  switch(activeAxisIndex) {
    case 0:
      activeAxis = &x;
      activeDisplay = &display_x;
      enc.write(*activeAxis * 4);
      break;
    case 1:
      activeAxis = &y;
      activeDisplay = &display_y;
      enc.write(*activeAxis * 4);
      break;
    case 2:
      activeAxis = &z;
      activeDisplay = &display_z;
      enc.write(*activeAxis * 4);
      break;
    default:
      activeAxis = &x;
      activeDisplay = &display_x;
      activeAxisIndex = 0;
      enc.write(*activeAxis * 4);
  }
}


void setup(void) {
  display_z.setup();
  display_x.setup();
  display_y.setup();

  pinMode(LED_BUILTIN, OUTPUT);
  switchAxis.attachClick(myClickFunction);
  switchAxis.attachDoubleClick(myDoubleClickFunction);
  switchAxis.attachLongPressStart(myLongPress);
}

void loop(void) {
  switchAxis.tick();
   long newPosition = enc.read() / 4;
   if (newPosition != *activeAxis) {
     *activeAxis = newPosition;
     (*activeDisplay).setValue(*activeAxis);
   }
}

