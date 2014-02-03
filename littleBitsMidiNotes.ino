#define NOTE_PIN 5
#define NOTE_TRIGGER_PIN 4
#define CC_PIN 6
#define CLOCK_PIN 3
#define SPEEDFACTOR 64
#define MIN_NOTE 24
#define MAX_NOTE MIN_NOTE+62

unsigned long count = 0;

const byte START = 250;
const byte STOP = 252;
const byte TICK = 248;
const byte CONTINUE = 251;

const byte NOTEON = 0x09;
const byte NOTEOFF = 0x08;
const byte CC = 0x0B;
const byte CLOCK = 0xF;

byte currentNote;

void setup() {
  // put your setup code here, to run once:
  pinMode(NOTE_PIN, OUTPUT);
  digitalWrite(NOTE_PIN, LOW);
  pinMode(NOTE_TRIGGER_PIN, OUTPUT);
  digitalWrite(NOTE_TRIGGER_PIN, LOW);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(CLOCK_PIN, HIGH);
}

void loop() {
  while(MIDIUSB.available() > 0) {
    MIDIEvent e = MIDIUSB.read();

    if(e.type == NOTEON){
      //Note to analog value
      //min note = 24
      if(e.m2 >= MIN_NOTE && e.m2 <= MAX_NOTE){
        // remember which note we're playing
        currentNote = e.m2;
        //stretch the note number across the correct range
        byte analogVal = map(currentNote, MIN_NOTE, MAX_NOTE, 0, 255);
        //output as a voltage
        analogWrite(NOTE_PIN, analogVal);
        //send a trigger pulse
        digitalWrite(NOTE_TRIGGER_PIN, HIGH);
        delay(1);
        digitalWrite(NOTE_TRIGGER_PIN, LOW);        
      }
    }
    if(e.type == NOTEOFF && e.m2 == currentNote){
      analogWrite(NOTE_PIN, 0);
    }
    if(e.type == CC && e.m2 == 1){ 
      // cc 1
      analogWrite(CC_PIN, e.m3 * 2);
    }
    // Clock pulses
    if(e.type == CLOCK){
      switch(e.m1){
      case TICK:
        {
          if((count % 24) == 0){
            digitalWrite(CLOCK_PIN, LOW);
            delay(1);
            digitalWrite(CLOCK_PIN, HIGH);
          }
          count++;
          break;
        }
      case START:
        {
          count = 0;
        }
      case STOP:
        {
          //ssssh
          digitalWrite(CLOCK_PIN, LOW);
        }
      }
    }
  }//end while midi available
}//end loop



