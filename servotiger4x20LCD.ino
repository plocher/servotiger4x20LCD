// Make music with servos and a Tiger Xylopiano
// by John Plocher
// Application: several servos with wooden hammers


#define hammerPin    14        // pwm outputs for servos
#define travellerPin 15
#define rxPin         5        // rxPin is immaterial - not used - just make this an unused Arduino pin number
#define txPin        16        // pin 16 is analog pin 3, on a BBB just use a servo cable
#define hoverpos     90        // "hover" position...
#define LCDline0    "?x00?y0"
#define LCDline1    "?x00?y1"
#define LCDline0p16 "?x15?y0"


#include <SoftwareSerial.h>
#include <Servo.h>

Servo hammer, traveller;
SoftwareSerial serialLCD =  SoftwareSerial(rxPin, txPin);

void setup(){
   pinMode(txPin, OUTPUT);
   serialLCD.begin(9600);      // 9600 baud is chip comm speed
   delayServo(2000);           // pause to allow LCD to display welcome
   serialLCD.print("?G420");   // set display geometry,
   serialLCD.print("?B7f");    // set backlight to ff hex, maximum brightness
   delayServo(110);
   serialLCD.print("?f?c0"); 
   delayServo(100);
   serialLCD.print("?x00?y2   XyloTiger    "); 
   serialLCD.print("?x00?y3by John Plocher "); 
   
   hammer.attach(hammerPin);         hammer.write(hoverpos);  
   traveller.attach(travellerPin);   traveller.write(hoverpos);  
   Servo::refresh();
   quiesce();
}

void loop() {  
   serialLCD.print(LCDline0); 
   serialLCD.print("Now Playing...  ");
   
   // serialLCD.print(LCDline1); delayServo(100);   
   // serialLCD.print("A Simple Scale  "); delayServo(100);
  // play("a b c d e f g A A2g2f2e2d2c2b2a  ");           // scale
   
   // serialLCD.print(LCDline1); serialLCD.print("Happy Birthday  ");
   //play("c4c2d6c6f6e82");     
  // play("c4c2d6c6g6f82");    
  // play("c4c2C6a6f6e6d62b4b2a6f6g6f");    

   serialLCD.print(LCDline1);  serialLCD.print("Mary had a Lamb ");
   play("e2d2c2d2e2e2e8d2d2d8e2e2e8e2d2c2d2e2e2e4e2d2d2e2d2c  ");     // Mary had a little lamb
   
   serialLCD.print(LCDline1); serialLCD.print("Row Row Row Boat"); 
   play("a6a6a2bc6c2bc2de66AAAeeecccaaae2dc2ba6 "); // Row Row your boat
   //serialLCD.print(LCDline1); serialLCD.print("Ode to Joy      "); 
   //play("e4e4f4g4g4f4e4d4c4c4d4e4e4d4d8e4e4f4g4g4f4e4d4c4c4d4e4d4c4c8  ");     // Ode to Joy
   serialLCD.print(LCDline1); serialLCD.print("Nano on bells   "); 
   play("ededcddcbcaeaeaeabcbcddcdede");         // Arduino on Bells tune
   serialLCD.print(LCDline1);   serialLCD.print("Muzak           "); 
   play("fdfdcdAdcgbcafafafgAeabcbcddcdfAdfgeca "); // Random...

   
   quiesce();
   
   serialLCD.print(LCDline0); 
   serialLCD.print("Press RESET to  "); 
   serialLCD.print(LCDline1);  
   serialLCD.print("play it again.  ");
   
   while (1) { }    // Play thru the set ONLY ONCE each reset...
}

void play(char *s) {
  int l = strlen(s);
  char n;
  for (int x = 0; x < l; x++) {
     switch (s[x]) {
       // Produce each note - positions determined empiricly
       case 'a': travelto(160); strike(); break;
       case 'b': travelto(138); strike(); break;
       case 'c': travelto(123); strike(); break;
       case 'd': travelto(107); strike(); break;
       case 'e': travelto(81);  strike(); break;
       case 'f': travelto(70);  strike(); break;
       case 'g': travelto(58);  strike(); break;
       case 'A': travelto(15);  strike(); break;
       case ' ': delayServo(1000);
       case '8': delayServo(100);
       case '6': delayServo(100);
       case '4': delayServo(100);
       case '2': delayServo(100); break;
     }
     if ((s[x] >= 'a' && s[x] <= 'g') || s[x] == 'A') {
       n = s[x];
     } else {
       n = ' ';
     }
     serialLCD.print(LCDline0p16);
     serialLCD.print(n);
     serialLCD.print("?h");
  }
}

int traveltime(int lastpos, int thispos) {
  return (abs(lastpos - thispos) * 7) + 40;
}


// Move hammer to the right note position
void travelto(int n) {
    static int lasttravelto = ' ';
    traveller.write(n);   
    delayServo(traveltime(lasttravelto, n));
    lasttravelto = n;
}

// Strike the note
void strike() {
    hammer.write(hoverpos + 7);   
    delayServo(30); 
    hammer.write(hoverpos);   
    delayServo(50);  
}

void quiesce() {
  travelto(160); // move hammer back to 'a'...
  hammer.write(hoverpos - 45 );  delayServo(1000);
  hammer.write(hoverpos);        delayServo(1000);
}

void delayServo(int t) {
  int x;
  Servo::refresh();
  for (x = 0; x < (t / 40); x++) {
    delay(40);
    Servo::refresh();
  }
  delay(t % 40);
  Servo::refresh();
}

  




  
